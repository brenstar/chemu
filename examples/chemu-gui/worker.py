from PyQt4 import QtCore
import PyChemu


class EmulatorWorker(QtCore.QThread):

    TIMER_FREQ = 1000 / 60.0

    class WorkerState:
        RUNNING = 0
        PAUSED = 1
        STOPPED = 2

    updateDisplay = QtCore.pyqtSignal(int, object)

    def __init__(self, emuWin):
        QtCore.QThread.__init__(self, emuWin)
        self.__emuWin = emuWin
        self.__emuWin.emu.contents.drawHandler = self._getDrawHandler()
        self.__emuWin.emu.contents.pollKeyHandler = self._getPollKeyHandler()
        self.__state = self.WorkerState.RUNNING
        self.__displayMutex = QtCore.QMutex()
        self.__displayWait = QtCore.QWaitCondition()
        self.__pollKeyMutex = QtCore.QMutex()
        self.__pollKeyWait = QtCore.QWaitCondition()
        self.__awaitingKey = False
        self.__delTimer = None
        self.__sndTimer = None
        self.started.connect(self.__threadStarted)

    def __threadStarted(self):
        if self.__state == self.WorkerState.RUNNING:
           self.__delTimer = self.startTimer(self.TIMER_FREQ)
           self.__sndTimer = self.startTimer(self.TIMER_FREQ)

    @property
    def displayWaitCondition(self):
        return self.__displayWait

    @property
    def pollKeyWaitCondition(self):
        return self.__pollKeyWait

    @property
    def awaitingKeyPress(self):
        return self.__awaitingKey

    def run(self):
        emu = self.__emuWin.emu
        if self.__state == self.WorkerState.RUNNING:
            while 1:
                emu.contents.input = self.__emuWin.input
                stepResult = PyChemu.chipemu_step(emu)
                if stepResult == PyChemu.CHIP_STEP_FAILURE:
                    print("fuck")
                    break
                # if emu.contents.dp.delTimer > 0:
                #     emu.contents.dp.delTimer -= 1
                # if emu.contents.dp.sndTimer > 0:
                #     emu.contents.dp.sndTimer -= 1
                # QtCore.QThread.msleep(16)
            self.__state = self.WorkerState.STOPPED

    def timerEvent(self, e):
        id = e.timerId()
        emu = self.__emuWin.emu
        if id == self.__delTimer:
            if emu.contents.dp.delTimer > 0:
                emu.contents.dp.delTimer -= 1
        elif id == self.__sndTimer:
            if emu.contents.dp.sndTimer > 0:
                emu.contents.dp.sndTimer -= 1

    def _getPollKeyHandler(self):
        def callback():
            self.__pollKeyMutex.lock()
            self.__awaitingKey = True
            self.__pollKeyWait.wait(self.__pollKeyMutex)
            self.__awaitingKey = False
            self.__pollKeyMutex.unlock()
            return self.__emuWin.lastChipKeyPressed
        return PyChemu.PollKeyHandler(callback)

    def _getDrawHandler(self):
        def callback(op, sprite):
            self.__displayMutex.lock()
            self.updateDisplay.emit(op, sprite)
            self.__displayWait.wait(self.__displayMutex)  # wait until display updates
            self.__displayMutex.unlock()
            return int(self.__emuWin.lastDrawHasCollision)
        return PyChemu.DrawHandler(callback)
