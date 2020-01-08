#!/usr/bin/env python3
import config as cfg
from webapp import app
from controller import controller

if __name__ == "__main__":
    app.run(host=cfg.server['host'], port=cfg.server['port'], debug=cfg.server['debug'])
