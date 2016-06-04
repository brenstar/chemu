from PyQt4 import QtCore, QtGui
from random import randint
import PyChemu
from ctypes import pointer


class ChipDisplayWidget(QtGui.QWidget):

    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.setMinimumSize(64, 32)
        self.__image = QtGui.QImage(64, 32, QtGui.QImage.Format_Mono)

        self.__testSprite = PyChemu.ChipSprite()
        self.__testSprite.x = 10
        self.__testSprite.y = 10
        self.__testSprite.rows = 5

        rowVal = 0b10101010
        for row in range(5):
            self.__testSprite.data[row] = rowVal
            rowVal = ~rowVal

        self.drawHandler(PyChemu.ChipDrawOp.CHIP_DRAW_CLEAR, None)
        self.drawHandler(PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE, pointer(self.__testSprite))
        #self.test()

    def test(self):
        self.__image.fill(0)
        for x in range(64):
            for y in range(32):
                self.__image.setPixel(x, y, randint(0, 1))
        self.__image.setPixel(10, 10, 1)
        self.repaint()

    def drawHandler(self, operation, sprite):
        if operation == PyChemu.ChipDrawOp.CHIP_DRAW_CLEAR:
            self.__image.fill(0)
        else:
            # draw sprite
            collision = False
            for rIndex in range(sprite.contents.rows):
                row = sprite.contents.data[rIndex]
                for col in range(8):
                    loc = QtCore.QPoint((sprite.contents.x + col) % 64,
                                        (sprite.contents.y + rIndex) % 32)
                    spritePixel = (row >> (7 - col)) & 1
                    curPixel = self.__image.pixelIndex(loc)
                    self.__image.setPixel(loc, spritePixel ^ curPixel)
                    if not collision and (spritePixel & curPixel) == 1:
                        collision = True
        self.repaint()

    def paintEvent(self, e):
        QtGui.QWidget.paintEvent(self, e)
        painter = QtGui.QPainter(self)
        sx = float(self.width()) / self.__image.width()
        sy = float(self.height()) / self.__image.height()
        painter.scale(sx, sy)
        painter.drawImage(0, 0, self.__image, 0, 0, 64, 32)

    #def mousePressEvent(self, e):
    #    self.test()

    def keyPressEvent(self, e):
        key = e.key()
        redraw = True

        if key in (QtCore.Qt.Key_W, QtCore.Qt.Key_A,
                   QtCore.Qt.Key_S, QtCore.Qt.Key_D):
            self.drawHandler(PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE, pointer(self.__testSprite))

        if key == QtCore.Qt.Key_W:
            self.__testSprite.y -= 1
        elif key == QtCore.Qt.Key_A:
            self.__testSprite.x -= 1
        elif key == QtCore.Qt.Key_S:
            self.__testSprite.y += 1
        elif key == QtCore.Qt.Key_D:
            self.__testSprite.x += 1
        else:
            redraw = False

        if redraw:
            self.drawHandler(PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE, pointer(self.__testSprite))



class EmulatorWindow(QtGui.QMainWindow):

    def __init__(self, emu, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.__emu = emu




if __name__ == "__main__":
    app = QtGui.QApplication([])

    #emu = PyChemu.chipemu_create()

    #win = EmulatorWindow(emu)
    #win.show()

    widget = ChipDisplayWidget()
    widget.resize(640, 320)
    widget.show()


    app.exec_()

    #PyChemu.chipemu_destroy(emu)
