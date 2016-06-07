from ctypes import *
import sys

if sys.platform == "linux" or sys.platform == "linux2":
    _libchemu = cdll.LoadLibrary("./chemu.so")
elif sys.platform == "win32":
    _libchemu = cdll.LoadLibrary("chemu.dll")


def enum(**vals):
    return type("Enum", (), vals)


# ChipDP.h ====================================================================

class ChipDP(Structure):
    _fields_ = [
        ("pc", c_uint16),
        ("addrReg", c_uint16),
        ("regs", c_uint8 * 16),
        ("sndTimer", c_uint8),
        ("delTimer", c_uint8)
    ]

# ChipInst.h ==================================================================

class ChipInst_AType(Structure):
    _fields_ = [
        ("addr", c_uint16, 12),
        ("reserved", c_uint16, 4)
    ]


class ChipInst_DType(Structure):
    _fields_ = [
        ("rows", c_uint16, 4),
        ("ry_num", c_uint16, 4),
        ("rx_num", c_uint16, 4),
        ("reserved", c_uint16, 4)
    ]


class ChipInst_IType(Structure):
    _fields_ = [
        ("immediate", c_uint16, 8),
        ("rnum", c_uint16, 4),
        ("reserved", c_uint16, 4)
    ]


class ChipInst_RType(Structure):
    _fields_ = [
        ("reserved_lo", c_uint16, 4),
        ("ra_num", c_uint16, 4),
        ("rs_num", c_uint16, 4),
        ("reserved_hi", c_uint16, 4)
    ]


class ChipInst(Union):
    _fields_ = [
        ("instruction", c_uint16),
        ("atype", ChipInst_AType),
        ("itype", ChipInst_IType),
        ("rtype", ChipInst_RType),
        ("dtype", ChipInst_DType)
    ]

#ChipInstFunc = CFUNCTYPE(c_int, POINTER(ChipEmu))

# ChipDisplay.h ==============================================================

class ChipSprite(Structure):
    _fields_ = [("x", c_uint8, 6),
                ("y", c_uint8, 5),
                ("rows", c_uint8, 4),
                ("data", c_uint8 * 15)]

ChipDrawOp = enum(
    CHIP_DRAW_CLEAR = 0,
    CHIP_DRAW_SPRITE = 1
)

DrawHandler = CFUNCTYPE(c_int, c_int, POINTER(ChipSprite))

# ChipInput.h ================================================================

ChipKey = enum(
    CHIP_KEY_0 = 0,
    CHIP_KEY_1 = 1,
    CHIP_KEY_2 = 2,
    CHIP_KEY_3 = 3,
    CHIP_KEY_4 = 4,
    CHIP_KEY_5 = 5,
    CHIP_KEY_6 = 6,
    CHIP_KEY_7 = 7,
    CHIP_KEY_8 = 8,
    CHIP_KEY_9 = 9,
    CHIP_KEY_A = 10,
    CHIP_KEY_B = 11,
    CHIP_KEY_C = 12,
    CHIP_KEY_D = 13,
    CHIP_KEY_E = 14,
    CHIP_KEY_F = 15
)

ChipKeyState = enum(
    CHIP_KEYSTATE_RELEASED = 0,
    CHIP_KEYSTATE_PRESSED = 1
)

ChipInput = c_uint16  # alias for uint16_t

PollInputHandler = CFUNCTYPE(None, POINTER(ChipInput))

PollKeyHandler = CFUNCTYPE(c_int)

# ChipMem.h ===================================================================

class ChipMem_reserved(Union):
    _fields_ = [
        ("array", c_uint8 * 512),
        ("fontset", c_uint8 * 80)
    ]

ChipMem_data = c_uint8 * 3584  # alias for uint8_t[3584]

class _ChipMem_inner(Structure):
    _fields_ = [
        ("reserved", ChipMem_reserved),
        ("data", ChipMem_data)
    ]

class ChipMem(Union):
    _anonymous_ = ("inner",)
    _fields_ = [
        ("array", c_uint8 * 4096),
        ("inner", _ChipMem_inner)
    ]

# ChipStack.h =================================================================

class ChipStack(Structure):
    _fields_ = [
        ("sp", POINTER(c_uint16)),
        ("data", c_uint16 * 16)
    ]

# ChipEmu.h ===================================================================

class ChipEmu(Structure):
    _fields_ = [
        ("dp", ChipDP),
        ("stack", ChipStack),
        ("memory", ChipMem),
        ("input", ChipInput),
        ("pollKeyHandler", PollKeyHandler),
        ("pollInputHandler", PollInputHandler),
        ("drawHandler", DrawHandler)
    ]

chipemu_create = _libchemu.chipemu_create
chipemu_create.argtypes = []
chipemu_create.restype = POINTER(ChipEmu)

chipemu_destroy = _libchemu.chipemu_destroy
chipemu_destroy.argtypes = [POINTER(ChipEmu)]
chipemu_destroy.restype = None

chipemu_mainLoop = _libchemu.chipemu_mainLoop
chipemu_mainLoop.argtypes = [POINTER(ChipEmu)]
chipemu_mainLoop.restype = c_int

chipin_keystate = _libchemu.chipin_keystate
chipin_keystate.argtypes = [POINTER(ChipInput), c_int]
chipin_keystate.restype = c_int

chipin_set = _libchemu.chipin_set
chipin_set.argtypes = [POINTER(ChipInput), c_int, c_int]
chipin_set.restype = c_int
