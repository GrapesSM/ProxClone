#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, LG_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class LaserGridController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)