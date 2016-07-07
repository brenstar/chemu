from PyQt4 import QtCore, QtGui
import PyChemu
from display import ChipDisplay, ChipDisplayWidget
from worker import EmulatorWorker
from ctypes import pointer, c_char_p
from enum import Enum





# displays the state of the chip keyboard
class ChipInputWidget(QtGui.QWidget):

    class InputButton(QtGui.QFrame):

        def __init__(self, key, inputWidget):
            QtGui.QFrame.__init__(self, inputWidget)
            self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
            self.customContextMenuRequested.connect(self.__showContextMenu)
            self.setAutoFillBackground(True)
            self.setFrameStyle(QtGui.QFrame.Plain | QtGui.QFrame.StyledPanel)
            self.__pals = tuple((QtGui.QPalette() for i in range(2)))
            self.__pals[0].setColor(QtGui.QPalette.Active, QtGui.QPalette.Window, QtCore.Qt.white)
            self.setPalette(self.__pals[0])
            self.__pals[1].setColor(QtGui.QPalette.Active, QtGui.QPalette.Window, QtCore.Qt.black)
            self.__pals[1].setColor(QtGui.QPalette.Active, QtGui.QPalette.WindowText, QtCore.Qt.white)
            self.__inputWidget = inputWidget
            self.__key = key
            self.__keyString = ("%x" % key).upper()
            self.__state = PyChemu.ChipKeyState.CHIP_KEYSTATE_RELEASED
            self.__cmenu = QtGui.QMenu()
            stickAction = self.__cmenu.addAction("Stick")
            stickAction.setCheckable(True)
            stickAction.triggered.connect(self.__stickAction)


        def __showContextMenu(self, point):
            self.__cmenu.exec_(self.mapToGlobal(point))

        def __stickAction(self, checked):
            if checked:
                self.__inputWidget.stick(self.__key)
            else:
                self.__inputWidget.unstick(self.__key)

        @property
        def state(self):
            return self.__state

        @state.setter
        def state(self, value):
            if value != self.__state:
                self.__state = value
                self.setPalette(self.__pals[int(value)])
                self.repaint()

        def mousePressEvent(self, e):
            if e.button() == QtCore.Qt.LeftButton:
                self.__inputWidget.setKeyState(self.__key, PyChemu.ChipKeyState.CHIP_KEYSTATE_PRESSED)

        def mouseReleaseEvent(self, e):
            if e.button() == QtCore.Qt.LeftButton:
                self.__inputWidget.setKeyState(self.__key, PyChemu.ChipKeyState.CHIP_KEYSTATE_RELEASED)


        def paintEvent(self, e):
            QtGui.QFrame.paintEvent(self, e)
            painter = QtGui.QPainter(self)
            painter.drawText(self.contentsRect(), QtCore.Qt.AlignCenter,
                             self.__keyString)


    KEYBOARD_LAYOUT = (
        PyChemu.ChipKey.CHIP_KEY_1,
        PyChemu.ChipKey.CHIP_KEY_2,
        PyChemu.ChipKey.CHIP_KEY_3,
        PyChemu.ChipKey.CHIP_KEY_C,
        PyChemu.ChipKey.CHIP_KEY_4,
        PyChemu.ChipKey.CHIP_KEY_5,
        PyChemu.ChipKey.CHIP_KEY_6,
        PyChemu.ChipKey.CHIP_KEY_D,
        PyChemu.ChipKey.CHIP_KEY_7,
        PyChemu.ChipKey.CHIP_KEY_8,
        PyChemu.ChipKey.CHIP_KEY_9,
        PyChemu.ChipKey.CHIP_KEY_E,
        PyChemu.ChipKey.CHIP_KEY_A,
        PyChemu.ChipKey.CHIP_KEY_0,
        PyChemu.ChipKey.CHIP_KEY_B,
        PyChemu.ChipKey.CHIP_KEY_F
    )

    inputChanged = QtCore.pyqtSignal(name="inputChanged")

    def __init__(self, emuWin):
        QtGui.QWidget.__init__(self, emuWin)
        emuWin.installEventFilter(self)
        self.setMinimumSize(100, 100)
        self.__input = PyChemu.ChipInput()
        self.__stuckKeys = set()
        self.__emuWin = emuWin


        # map of Qt keycodes to chip keycodes
        self.__keymap = {
            QtCore.Qt.Key_1 : PyChemu.ChipKey.CHIP_KEY_1,
            QtCore.Qt.Key_2 : PyChemu.ChipKey.CHIP_KEY_2,
            QtCore.Qt.Key_3 : PyChemu.ChipKey.CHIP_KEY_3,
            QtCore.Qt.Key_4 : PyChemu.ChipKey.CHIP_KEY_C,
            QtCore.Qt.Key_Q : PyChemu.ChipKey.CHIP_KEY_4,
            QtCore.Qt.Key_W : PyChemu.ChipKey.CHIP_KEY_5,
            QtCore.Qt.Key_E : PyChemu.ChipKey.CHIP_KEY_6,
            QtCore.Qt.Key_R : PyChemu.ChipKey.CHIP_KEY_D,
            QtCore.Qt.Key_A : PyChemu.ChipKey.CHIP_KEY_7,
            QtCore.Qt.Key_S : PyChemu.ChipKey.CHIP_KEY_8,
            QtCore.Qt.Key_D : PyChemu.ChipKey.CHIP_KEY_9,
            QtCore.Qt.Key_F : PyChemu.ChipKey.CHIP_KEY_E,
            QtCore.Qt.Key_Z : PyChemu.ChipKey.CHIP_KEY_A,
            QtCore.Qt.Key_X : PyChemu.ChipKey.CHIP_KEY_0,
            QtCore.Qt.Key_C : PyChemu.ChipKey.CHIP_KEY_B,
            QtCore.Qt.Key_V : PyChemu.ChipKey.CHIP_KEY_F
        }

        self.__layout = QtGui.QGridLayout()
        self.__keyWidgets = [None for i in range(16)]
        for i, key in enumerate(self.KEYBOARD_LAYOUT):
            widget = ChipInputWidget.InputButton(key, self)
            self.__layout.addWidget(widget, i // 4, i % 4)
            self.__keyWidgets[key] = widget
        self.setLayout(self.__layout)




    @property
    def input(self):
        return self.__input

    # sets the given key to be stuck, stuck keys will always have a keystate
    # of pressed
    def stick(self, key):
        if key not in self.__stuckKeys:
            self.__stuckKeys.add(key)
            self.setKeyState(key, PyChemu.ChipKeyState.CHIP_KEYSTATE_PRESSED)

    def unstick(self, key):
        if key in self.__stuckKeys:
            self.__stuckKeys.remove(key)
            self.setKeyState(key, PyChemu.ChipKeyState.CHIP_KEYSTATE_RELEASED)

    def eventFilter(self, obj, event):
        # pass keyboard events to this widget's handlers
        if obj is self.__emuWin:
            if event.type() == QtCore.QEvent.KeyPress:
                print("press")
                self.keyPressEvent(event)
                #return True
            elif event.type() == QtCore.QEvent.KeyRelease:
                self.keyReleaseEvent(event)
                #return True
        return False

    def __updateInput(self, key, state):
        if key in self.__keymap:
            chipkey = self.__keymap[key]
            if chipkey not in self.__stuckKeys:
                self.setKeyState(chipkey, state)
                self.inputChanged.emit()
                self.repaint()

    def setKeyState(self, key, state):
        PyChemu.chipin_set(pointer(self.__input), key, state)
        self.__keyWidgets[key].state = state

    def keyPressEvent(self, e):
        if e.isAutoRepeat():
            return
        self.__updateInput(e.key(), PyChemu.ChipKeyState.CHIP_KEYSTATE_PRESSED)

    def keyReleaseEvent(self, e):
        if e.isAutoRepeat():
            return
        self.__updateInput(e.key(), PyChemu.ChipKeyState.CHIP_KEYSTATE_RELEASED)

    def paintEvent(self, e):
        QtGui.QWidget.paintEvent(self, e)
        return
        painter = QtGui.QPainter(self)

        boxSize = 20
        for i, key in enumerate(self.__layout):
            x = (i % 4) * boxSize
            y = (i // 4) * boxSize
            if PyChemu.chipin_keystate(pointer(self.__input), key) == PyChemu.ChipKeyState.CHIP_KEYSTATE_PRESSED:
                painter.fillRect(x, y, boxSize, boxSize, QtCore.Qt.SolidPattern)
            else:
                painter.drawRect(x, y, boxSize, boxSize)




class EmulatorWindow(QtGui.QMainWindow):


    def __init__(self, emu, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.__emu = emu
        self.__emu.contents.drawHandler = self.getDrawHandlerCallback()
        #self.__worker = EmulatorWorker(self)
        #self.__worker.updateDisplay.connect(self.__updateDisplay)
        self.setWindowTitle("CHIP-8 Emulator")
        self.resize(640, 320)

        # central widget
        self.__display = ChipDisplay(self)
        self.__displayWidget = ChipDisplayWidget(self.__display, parent=self)
        self.setCentralWidget(self.__displayWidget)

        # execution control dock
        # self.__exeCtrlDock = QtGui.QDockWidget("Execution Control", self)
        # self.__currentInstLabel = QtGui.QLabel()
        # self.__testLabel = QtGui.QLabel()
        # exeWidget = QtGui.QWidget(self.__exeCtrlDock)
        # layout = QtGui.QGridLayout(exeWidget)
        # layout.addWidget(QtGui.QLabel("Next Instruction:"), 0, 0)
        # layout.addWidget(self.__currentInstLabel, 0, 1)
        # layout.addWidget(self.__testLabel, 1, 0)
        # self.__exeCtrlDock.setWidget(exeWidget)
        # self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.__exeCtrlDock)
        #self.__exeCtrlDock.hide()

        # emulation state dock
        self.__emuStateDock = QtGui.QDockWidget("Emulator State", self)
        self.__nextInstLabel = QtGui.QLabel()
        self.__pcLabel = QtGui.QLabel()
        self.__stLabel = QtGui.QLabel()
        self.__dtLabel = QtGui.QLabel()
        # self.__regTable = QtGui.QTreeWidget()
        # self.__regTable.setHeaderLabels(["Register", "Value"])
        # for reg in ["v{0:X}".format(i) for i in range(16)] + ["I"]:
        #     item = QtGui.QTreeWidgetItem([reg, "0"])
        #     self.__regTable.addTopLevelItem(item)
        stateWidget = QtGui.QWidget(self.__emuStateDock)
        layout = QtGui.QGridLayout()
        layout.addWidget(QtGui.QLabel("Next Instruction: "), 0, 0)
        layout.addWidget(self.__nextInstLabel, 0, 1)
        layout.addWidget(QtGui.QLabel("PC:"), 1, 0)
        layout.addWidget(self.__pcLabel, 1, 1)
        layout.addWidget(QtGui.QLabel("ST:"), 1, 2)
        layout.addWidget(self.__stLabel, 1, 3)
        layout.addWidget(QtGui.QLabel("DT:"), 1, 4)
        layout.addWidget(self.__dtLabel, 1, 5)
        # layout.addWidget(QtGui.QLabel("Registers"), 2, 0)
        # layout.addWidget(self.__regTable, 3, 0, 1, 6)

        stateWidget.setLayout(layout)
        self.__emuStateDock.setWidget(stateWidget)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.__emuStateDock)

        # keyboard dock
        self.__keyboardDock = QtGui.QDockWidget("Keyboard", self)
        self.__inputWidget = ChipInputWidget(self)
        self.__keyboardDock.setWidget(self.__inputWidget)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.__keyboardDock)

        self.__input = PyChemu.ChipInput()
        self.__lastKeyPressed = 0
        self.__lastCollision = 0

        self.__stepTimer = None
        self.__sndDelTimer = None

        self.__testSprite = PyChemu.ChipSprite()
        self.__testSprite.x = 24
        self.__testSprite.y = 12
        self.__testSprite.rows = 4
        for i in range(4):
            self.__testSprite.data[i] = 0b00111100
        #self.__display.drawSprite(self.__testSprite)

    @property
    def input(self):
        return self.__inputWidget.input

    @property
    def emu(self):
        return self.__emu

    @property
    def lastDrawHasCollision(self):
        return self.__lastCollision

    @property
    def lastChipKeyPressed(self):
        return self.__lastKeyPressed

    def getDrawHandlerCallback(self):
        def callback(op, sprite):
            coll = 0
            if op == PyChemu.ChipDrawOp.CHIP_DRAW_CLEAR.value:
                self.__display.clear()
            elif op == PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE.value:
                if self.__display.drawSprite(sprite.contents):
                    coll = 1
            return coll

        return PyChemu.DrawHandler(callback)

    def getPollKeyHandlerCallback(self):
        def callback():
            return 0
        return PyChemu.PollKeyHandler(callback)

    def __updateTestLabel(self):
        self.__testLabel.setText("{0:016b}".format(self.__input.value))

    def __updateStateDock(self):
        dp = self.__emu.contents.dp
        nextinst = (self.__emu.contents.memory.array[dp.pc] << 8) | \
                   self.__emu.contents.memory.array[dp.pc + 1]
        self.__nextInstLabel.setText("0x{0:04X}".format(nextinst))
        self.__pcLabel.setText("0x{0:03X}".format(dp.pc))
        self.__stLabel.setText(str(dp.sndTimer))
        self.__dtLabel.setText(str(dp.delTimer))


    def __updateDisplay(self, op, sprite):
        self.__lastCollision = 0
        if op == PyChemu.ChipDrawOp.CHIP_DRAW_CLEAR:
            self.__display.clear()
        elif op == PyChemu.ChipDrawOp.CHIP_DRAW_SPRITE:
            if self.__display.drawSprite(sprite.contents):
                self.__lastCollision = 1
        self.__worker.displayWaitCondition.wakeAll()

    def keyPressEvent(self, e):
        if e.key() == QtCore.Qt.Key_P:
            self.__stepTimer = self.startTimer(10)
            self.__sndDelTimer = self.startTimer(16)
        # if e.isAutoRepeat():
        #     return
        #
        # key = e.key()
        #
        # if not self.__worker.isRunning() and e.key() == QtCore.Qt.Key_P:
        #     print("Starting thread")
        #     self.__worker.start()
        #
        # if e.key() == QtCore.Qt.Key_T:
        #     self.__display.drawSprite(self.__testSprite)
        #     self.__testSprite.x += 1
        #     self.__display.drawSprite(self.__testSprite)
        #
        #
        # if key in self.__keymap:
        #     chipkey = self.__keymap[key]
        #     if self.__worker.awaitingKeyPress:
        #         self.__lastKeyPressed = chipkey
        #         self.__worker.pollKeyWaitCondition.wakeAll()
        #     PyChemu.chipin_set(pointer(self.__input), chipkey,
        #                        PyChemu.ChipKeyState.CHIP_KEYSTATE_PRESSED)
        #     self.__updateTestLabel()
            #print(self.__input)
            #print("Key %d is pressed" % chipkey)


    def keyReleaseEvent(self, e):
        pass
        # if e.isAutoRepeat():
        #     return
        # key = e.key()
        # if key in self.__keymap:
        #     chipkey = self.__keymap[key]
        #     PyChemu.chipin_set(pointer(self.__input), chipkey,
        #                        PyChemu.ChipKeyState.CHIP_KEYSTATE_RELEASED)
        #     self.__updateTestLabel()
            #print("Key %d is released" % chipkey)
            #print(self.__input)

    def timerEvent(self, e):
        id = e.timerId()
        if id == self.__stepTimer:
            self.__emu.contents.input = self.__inputWidget.input
            PyChemu.chipemu_step(self.__emu)
            self.__updateStateDock()
        elif id == self.__sndDelTimer:
            if self.__emu.contents.dp.delTimer > 0:
                self.__emu.contents.dp.delTimer -= 1
            if self.__emu.contents.dp.sndTimer > 0:
                self.__emu.contents.dp.sndTimer -= 1


if __name__ == "__main__":
    app = QtGui.QApplication([])
    app.setStyleSheet("""
        #ChipDisplayWidget {
            background-color: #404040;
        }

    """)

    emu = PyChemu.chipemu_create()
    # for testing purposes
    if PyChemu.chipemu_loadROM(emu, c_char_p(b"invaders.c8")) == 1:
       print("Failed to load ROM")

    # this program will test the font sprites
    # loops from 0 to 15 and draws each font sprite
    # testProg = [
    #     0x60, 0x00,     # li v0, 0
    #     0x61, 0x0a,     # li v1, 10
    #     0x62, 0x0a,     # li v2, 10
    #     0xf0, 0x29,     # font v0
    #     0xD1, 0x25,     # draw v1, v2, 5
    #     0x70, 0x01,     # addi v0, 1
    #     0x40, 0x10,     # snei v0, 16
    #     0x60, 0x00,     # li v0, 0
    #     0x00, 0xe0,     # cls
    #     0x12, 0x06      # j 0x206
    # ]

    # testProg = [
    #     0x6a, 0x00,     #li vA, 0
    #     0x6b, 0x0a,     #li vB, 10
    #     0x6c, 0x0a,     #li vC, 10
    #     0xaf, 0x00,     #la 0xf00
    #     0xfa, 0x33,     #bcd vA
    #     0xf2, 0x65,     # rest v2
    #     0xf0, 0x29,     #font v0
    #     0xdb, 0xc5,     #draw vB, vC, 5
    #     0x7b, 0x0a,     #addi vB, 1
    #     0xf1, 0x29,     #font v1
    #     0xdb, 0xc5,     #draw vB, vC, 5
    #     0x7b, 0x0a,     #addi vB, 1
    #     0xf2, 0x29,     #font v2
    #     0xdb, 0xc5,     #draw vB, vC, 5
    #     0x6b, 0x0a,     #li vB, 10
    #     0x7a, 0x01,     #addi vA, 1
    #     0x00, 0xe0,     # cls
    #     0x12, 0x06      # j 0x206
    # ]
    #
    # for i in range(len(testProg)):
    #     emu.contents.memory.data[i] = testProg[i]

    win = EmulatorWindow(emu)
    win.show()

    #widget = ChipDisplayWidget()
    #widget.resize(640, 320)
    #widget.show()


    app.exec_()

    PyChemu.chipemu_destroy(emu)
