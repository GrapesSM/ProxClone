#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, SB_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class StatusBoardController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)