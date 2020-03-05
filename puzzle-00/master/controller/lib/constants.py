from enum import IntEnum
class STATE:
  OFF = 0
  ON = 1
  ACTIVE = 2
  STANDBY = 3
  SOLVED = 4
  OFFLINE = 5
  FAILURE = 6
  SYSTEM_ERROR = 7
  ERROR = 8
  SYNCRONIZED = 9
  INITIALIZING = 10
  INITIALIZED = 11
  START = 12
  END = 13
  READING = 14
  PLAYING = 15
  BALANCED = 16
  SETUP = 17
  FLASH = 18
  UNBALANCED = 19
  ALARM = 20
  RESET = 21
  UNSOLVED = 22
  DONE = 23
  DISABLE = 24
  ENABLE = 25
  PAUSE = 26
  COUNTING = 27
  START_TIMER = 28
  CHARGING = 29
  FULL = 30
  EMPTY = 31
  OPEN = 32
  CLOSED = 33
  NODATA = 34

class COMMAND:
  CMD_NONE = 0
  CMD_ENABLE = 1
  CMD_DISABLE = 2
  CMD_RESET = 3
  CMD_PAUSE = 4
  CMD_SET_DEMAND = 5
  STATUS_NONE = 6
  STATUS_CREATED = 7
  STATUS_CONFIRMED = 8
  STATUS_ERROR = 9 
  CMD_SET_BATERRY_MATRIX_SOLVED = 10

class REGISTER_INDEX:
  REG_MASTER_MILLIS = 0
  REG_MASTER_COMMAND = 1
  REG_MASTER_FORCE = 2
  REG_SLAVE_MILLIS = 3
  REG_SLAVE_CONFIRM = 4
  REG_SLAVE_STATE = 5

class DM_REGISTER_INDEX(REGISTER_INDEX):
  REG_SLAVE_POWER_SWITCH_STATE = 6
  REG_SLAVE_CODE_READER_STATE = 7
  REG_SLAVE_INFORMATION_DISPLAY_STATE = 8
  REG_SLAVE_LIGHT_EFFECT_STATE = 9
  REG_SLAVE_SPEAKER_STATE = 10

class PC_REGISTER_INDEX(REGISTER_INDEX):
  REG_SLAVE_POWER_ADJUSTER_STATE = 6
  REG_SLAVE_BATTERY_STATE = 7
  REG_SLAVE_POWER_LIGHT_INDICATOR_STATE = 8
  REG_SLAVE_SPEAKER_STATE = 9
  REG_SLAVE_LIGHT_EFFECT_STATE = 10
  REG_SLAVE_BATTERY = 11
  REG_SLAVE_SUPPLY = 12
  REG_SLAVE_DEMAND = 13
  REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER = 14

class DS_REGISTER_INDEX:
  REG_MASTER_MILLIS = 0
  REG_MASTER_ES_COMMAND = 1
  REG_MASTER_ES_FORCE = 2
  REG_SLAVE_ES_MILLIS = 3
  REG_SLAVE_ES_CONFIRM = 4
  REG_SLAVE_ES_STATE = 5
  REG_SLAVE_ES_POWER_SWITCH_STATE = 6
  REG_SLAVE_ES_BATTERY_MATRIX_STATE = 7
  REG_SLAVE_ES_GENERATOR_STATE = 8
  REG_SLAVE_ES_SPEAKER_STATE = 9
  ################################
  REG_MASTER_SP_COMMAND = 10
  REG_MASTER_SP_FORCE = 11
  REG_SLAVE_SP_MILLIS = 12
  REG_SLAVE_SP_CONFIRM = 13
  REG_SLAVE_SP_STATE = 14
  REG_SLAVE_SP_POWER_SWITCH_STATE = 15
  REG_SLAVE_SP_POWER_ADJUSTER_STATE = 16
  REG_SLAVE_SP_SYNCRO_READER_STATE = 17
  REG_SLAVE_SP_SPEAKER_STATE = 18


class LG_REGISTER_INDEX(REGISTER_INDEX):
  REG_SLAVE_POWER_SWITCH_STATE = 6
  REG_SLAVE_KEY_READER_STATE = 7
  REG_SLAVE_WAVE_ADJUSTER_STATE = 8
  REG_SLAVE_SPEAKER_STATE = 9

class PS_REGISTER_INDEX(REGISTER_INDEX):
  REG_SLAVE_POWER_SWITCH_STATE = 6
  REG_SLAVE_BATTERY_MATRIX_STATE = 7
  REG_SLAVE_ENERGY_SUPP_STATE = 8
  REG_SLAVE_GENERATOR_STATE = 9
  REG_SLAVE_SYNCRO_READER_STATE = 10
  REG_SLAVE_SPEAKER_STATE = 11
  REG_SLAVE_LIGHT_EFFECT_STATE = 12
  REG_SLAVE_SYNCRO_READER_INPUT_KEY = 13

class SC_REGISTER_INDEX(REGISTER_INDEX):
  REG_SLAVE_POWER_SWITCH_STATE = 6
  REG_SLAVE_ACCESS_PANEL_STATE = 7
  REG_SLAVE_COMBINATION_READER_STATE = 8
  REG_SLAVE_DOOR_STATE = 9
  REG_SLAVE_SPEAKER_STATE = 10