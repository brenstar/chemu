from PyQt4 import QtCore, QtGui
import PyChemu


class ChipDisplayWidget(QtGui.QWidget):

    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.setMinimumSize(64, 32)
        self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.__showContextMenu)
        self.__image = QtGui.QImage(64, 32, QtGui.QImage.Format_Mono)
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


        self.clear()

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

    def clear(self):
        self.__image.fill(0)
        self.repaint()

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
        self.repaint()
        return collision

    def paintEvent(self, e):
        QtGui.QWidget.paintEvent(self, e)
        painter = QtGui.QPainter(self)
        if self.__stretch:
            sx = float(self.width()) / self.__image.width()
            sy = float(self.height()) / self.__image.height()
            if self.__keepAspectRatio:
                new_sy = sx / 2
                if new_sy > sy:
                    sx = sy * 2
                else:
                    sy = new_sy
        else:
            sx = self.__fixedScale
            sy = self.__fixedScale
        x = (self.width() - (self.__image.width() * sx)) / 2.0
        y = (self.height() - (self.__image.height() * sy)) / 2.0
        painter.translate(x, y)
        painter.scale(sx, sy)
        painter.drawImage(0, 0, self.__image)



class EmulatorWindow(QtGui.QMainWindow):

    def __init__(self, emu, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.__emu = emu
        self.setWindowTitle("CHIP-8 Emulator")
        self.resize(640, 480)

        # central widget
        self.__display = ChipDisplayWidget(parent=self)
        self.setCentralWidget(self.__display)

        # execution control dock
        self.__exeCtrlDock = QtGui.QDockWidget("Execution Control", self)
        self.__currentInstLabel = QtGui.QLabel()
        exeWidget = QtGui.QWidget(self.__exeCtrlDock)
        layout = QtGui.QGridLayout(exeWidget)
        layout.addWidget(QtGui.QLabel("Next Instruction:"), 0, 0)
        layout.addWidget(self.__currentInstLabel, 0, 1)
        self.__exeCtrlDock.setWidget(exeWidget)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.__exeCtrlDock)



    def getDrawHandlerCallback(self):
        def callback(operation, sprite):
            coll = 0
            if op == PyChemu.ChipDrawOp.CHIP_DRAW_CLEAR:
                self.__display.clear()
            elif op == PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE:
                if self.__display.drawSprite(sprite):
                    coll = 1
            return coll

        return PyChemu.DrawHandler(callback)



if __name__ == "__main__":
    app = QtGui.QApplication([])

    #emu = PyChemu.chipemu_create()

    win = EmulatorWindow(None)
    win.show()

    #widget = ChipDisplayWidget()
    #widget.resize(640, 320)
    #widget.show()


    app.exec_()

    #PyChemu.chipemu_destroy(emu)
