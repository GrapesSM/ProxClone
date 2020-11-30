#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
from .base_controller import BaseController
from .constants import STATE, COMMAND, PS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum
import time

class PrepStatusController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)