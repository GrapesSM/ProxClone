#!/usr/bin/etc python3
from .base_controller import BaseController

class KeypadController(BaseController):
    def __init__(self, key_name,model, puzzle):
        super().__init__(key_name,model, puzzle)