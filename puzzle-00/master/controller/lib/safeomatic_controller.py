#!/usr/bin/etc python3
from base_controller import BaseController

class SafeomaticController(BaseController):
    def __init__(self, model, slave):
        super().__init__(model, slave)
