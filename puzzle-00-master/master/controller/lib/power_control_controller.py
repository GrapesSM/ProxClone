#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PC_REGISTER_INDEX, STATUS
from .helpers import time_now

class PowerControlController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super(key_name, model, puzzle, master)
        self._demand = 0
        self._lighEffectPatternNumber = 0
        self._baterryMaxValue = 13
        self._batteryChargingRate = 7
        self._failurePeriodValue = 3