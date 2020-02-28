#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, DATAMATIC_REGISTER_INDEX
from enum import Enum


class DatamaticController(BaseController):
    
    def __init__(self, key_name,model, puzzle):
        super().__init__(key_name, model, puzzle)
        
    def update(self):
        
    
    


