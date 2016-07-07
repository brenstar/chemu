from PyQt4 import QtGui, QtCore
from enum import Enum
import PyChemu


class ChipDisplay(QtCore.QObject):

    WIDTH = 64
    HEIGHT = 32

    displayRedraw = QtCore.pyqtSignal()

    def __init__(self, parent=None):
        QtCore.QObject.__init__(self, parent)
        self.__image = QtGui.QImage(self.WIDTH, self.HEIGHT, QtGui.QImage.Format_Mono)
        self.__table = self.__image.colorTable()
        # flicker prevention
        self.__fpEnabled = True
        # cache of all sprites drawn, keys being coordinates and values
        # being sprite data
        # (int, int) -> (int, [int])
        self.__spriteCache = {}

    @property
    def flickerPrevent(self):
        return self.__fpEnabled

    @flickerPrevent.setter
    def flickerPrevent(self, val):
        self.__fpEnabled = val

    @property
    def image(self):
        return self.__image

    @property
    def backgroundColor(self):
        return QtGui.QColor(self.__table[0])

    @backgroundColor.setter
    def backgroundColor(self, value):
        self.__table[0] = value.rgb()
        self.__image.setColorTable(self.__table)
        self.displayRedraw.emit()

    @property
    def foregroundColor(self):
        return QtGui.QColor(self.__table[1])

    @foregroundColor.setter
    def foregroundColor(self, value):
        self.__table[1] = value.rgb()
        self.__image.setColorTable(self.__table)
        self.displayRedraw.emit()

    def clear(self):
        self.__image.fill(0)
        if self.__fpEnabled:
            self.__spriteCache.clear()
        self.displayRedraw.emit()

    def drawSprite(self, sprite):
        collision = False
        for rIndex in range(sprite.rows):
            row = sprite.data[rIndex]
            for col in range(8):
                loc = QtCore.QPoint((sprite.x + col) % 64,
                                    (sprite.y + rIndex) % 32)
                spritePixel = (row >> (7 - col)) & 1
                curPixel = self.__image.pixelIndex(loc)
                self.__image.setPixel(loc, spritePixel ^ curPixel)
                if not collision and (spritePixel & curPixel) == 1:
                    collision = True
        redraw = True
        if self.__fpEnabled:
            coord = sprite.x, sprite.y
            if coord in self.__spriteCache:
                # check if erased
                rows, data = self.__spriteCache[coord]
                if rows == sprite.rows:
                    erased = True
                    for i in range(rows):
                        if data[i] != sprite.data[i]:
                            erased = False  # sprite data is different
                    if erased:
                        del self.__spriteCache[coord]
                        redraw = False
            else:
                # add sprite to cache
                data = [row for row in sprite.data]
                #print(data)
                self.__spriteCache[coord] = (sprite.rows, data)
            # if self.__fpstate == self._FPState.INIT:
            #     #self.__fpLastSprite = sprite
            #     self.__fpstate = self._FPState.CHECK
            # elif self.__fpstate == self._FPState.CHECK:
            #     if self.__fpLastSprite == sprite:
            #         # this sprite was drawn previously, so it has been erased
            #         redraw = False  # don't redraw just yet
            #         self.__fpstate = self._FPState.WAIT
            #     #else:
            #     #    self.__fpLastSprite = sprite
            # elif self.__fpstate == self._FPState.WAIT:
            #     #self.__fpLastSprite = sprite
            #     self.__fpstate = self._FPState.CHECK
            #
            # copy = PyChemu.ChipSprite()
            # copy.x = sprite.x
            # copy.y = sprite.y
            # copy.rows = sprite.rows
            # for row in range(sprite.rows):
            #     copy.data[row] = sprite.data[row]
            # self.__fpLastSprite = copy

        if redraw:
            self.displayRedraw.emit()
        #self.__refreshDisplay = True
        return collision



class ChipDisplayWidget(QtGui.QWidget):


    def __init__(self, display, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.setObjectName("ChipDisplayWidget")
        self.setMinimumSize(64, 32)
        self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__showContextMenu)

        self.__display = display
        self.__display.displayRedraw.connect(lambda: self.repaint())

        self.__RATIO = float(self.__display.image.width()) / self.__display.image.height()
        self.__fixedScale = 1
        self.__stretch = True
        self.__keepAspectRatio = True

        # Context menu
        self.__cmenu = QtGui.QMenu("Context menu", self)
        self.__sizeActionGroup = QtGui.QActionGroup(self)

        arAction = self.__cmenu.addAction("Keep aspect ratio")
        arAction.setCheckable(True)
        arAction.setChecked(True)
        arAction.triggered.connect(self.__arActionTriggered)

        self.__cmenu.addSeparator()

        stretchAction = self.__cmenu.addAction("Stretch to fit")
        stretchAction.setCheckable(True)
        stretchAction.setChecked(True)
        stretchAction.triggered.connect(self.__stretchActionTriggered)
        self.__sizeActionGroup.addAction(stretchAction)

        s1 = self.__cmenu.addAction("1x")
        s1.setCheckable(True)
        s1.triggered.connect(lambda: self.__scaleActionTriggered(1))
        self.__sizeActionGroup.addAction(s1)
        s2 = self.__cmenu.addAction("2x")
        s2.setCheckable(True)
        s2.triggered.connect(lambda: self.__scaleActionTriggered(2))
        self.__sizeActionGroup.addAction(s2)
        s3 = self.__cmenu.addAction("3x")
        s3.setCheckable(True)
        s3.triggered.connect(lambda: self.__scaleActionTriggered(3))
        self.__sizeActionGroup.addAction(s3)
        s4 = self.__cmenu.addAction("4x")
        s4.setCheckable(True)
        s4.triggered.connect(lambda: self.__scaleActionTriggered(4))
        self.__sizeActionGroup.addAction(s4)

        self.__display.clear()

    def __showContextMenu(self, point):
        self.__cmenu.exec_(self.mapToGlobal(point))

    def __arActionTriggered(self, checked):
        self.keepAspectRatio = checked

    def __stretchActionTriggered(self):
        self.__stretch = True
        self.repaint()

    def __scaleActionTriggered(self, amount):
        if self.__stretch:
            self.__stretch = False
        self.scale = amount

    @property
    def scale(self):
        return self.__fixedScale

    @scale.setter
    def scale(self, value):
        if value > 0:
            self.__fixedScale = value
            if not self.__stretch:
                self.repaint()

    @property
    def keepAspectRatio(self):
        return self.__keepAspectRatio

    @keepAspectRatio.setter
    def keepAspectRatio(self, val):
        old = self.__keepAspectRatio
        self.__keepAspectRatio = val
        if old != val:
            self.repaint()

    @property
    def stretchToFit(self):
        return self.__stretch

    @stretchToFit.setter
    def stretchToFit(self, value):
        self.__stretch = value
        self.repaint()

    def paintEvent(self, e):
        QtGui.QWidget.paintEvent(self, e)
        painter = QtGui.QPainter(self)
        painter.fillRect(self.contentsRect(),
                         self.palette().color(QtGui.QPalette.Active,
                                              QtGui.QPalette.Window))
        img = self.__display.image
        if self.__stretch:
            width = self.width()
            height = self.height()
            if self.__keepAspectRatio:
                ratio = float(self.width()) / self.height()
                if ratio > self.__RATIO:  # width is too big
                    width = height * self.__RATIO
                elif ratio < self.__RATIO:  # height is too big
                    height = width / self.__RATIO

            sx = float(width) / img.width()
            sy = float(height) / img.height()
        else:
            sx = self.__fixedScale
            sy = self.__fixedScale
        x = (self.width() - (img.width() * sx)) / 2.0
        y = (self.height() - (img.height() * sy)) / 2.0
        painter.translate(x, y)
        painter.scale(sx, sy)
        painter.drawImage(0, 0, img)


class DisplayTest(QtCore.QObject):

    def __init__(self, disp, parent=None):
        QtCore.QObject.__init__(self, parent)

        self.__disp = disp

        self.__sprite = PyChemu.ChipSprite()
        self.__sprite.x = 10
        self.__sprite.y = 10
        self.__sprite.rows = 4
        for i in range(4):
            self.__sprite.data[i] = 0xf0

        disp.drawSprite(self.__sprite)

    def eventFilter(self, obj, evt):
        if evt.type() == QtCore.QEvent.KeyPress:
            key = evt.key()

            if key == QtCore.Qt.Key_P:
                self.__disp.flickerPrevent = not self.__disp.flickerPrevent
                return True

            coords = [self.__sprite.x, self.__sprite.y]
            if key == QtCore.Qt.Key_W:
                coords[1] -= 1
            if key == QtCore.Qt.Key_A:
                coords[0] -= 1
            if key == QtCore.Qt.Key_S:
                coords[1] += 1
            if key == QtCore.Qt.Key_D:
                coords[0] += 1

            self.__disp.drawSprite(self.__sprite)
            self.__sprite.x, self.__sprite.y = coords
            self.__disp.drawSprite(self.__sprite)
            return True
        return False


if __name__ == "__main__":
    app = QtGui.QApplication([])

    disp = ChipDisplay()
    disp.foregroundColor = QtGui.QColor(255, 0, 0)
    disp.backgroundColor = QtGui.QColor(0, 128, 128)
    win = ChipDisplayWidget(disp)
    win.resize(640, 480)
    win.show()

    test = DisplayTest(disp)
    win.installEventFilter(test)

    app.exec_()
