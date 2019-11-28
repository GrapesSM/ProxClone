#include <NeoPixelBus.h>

// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);      
class Grid 
{
    public:
        Grid();
    protected:
        uint8_t rows;
        uint8_t columns;
};

Grid::Grid() {
    rows = 0;
    columns = 0;
}

class LightGrid : public Grid
{
    public:
        LightGrid();
        LightGrid(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * p, uint8_t length);
        LightGrid(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * p, uint8_t rows, uint8_t columns);

        void setColor();
        void setColor(uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint8_t number, uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint8_t row, uint8_t column, uint8_t red, uint8_t green, uint8_t blue);
        void setColor(uint8_t box[], uint8_t rows, uint8_t columns);
        uint8_t* getColor(uint8_t row, uint8_t column);
    private:
        void _setColor();

        NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _p;
        uint8_t * _box;
};

LightGrid::LightGrid() {
    this->_p = NULL;
    this->_box = NULL;
}

LightGrid::LightGrid(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * p, uint8_t length) {
    this->rows = 1;
    this->columns = length;
    this->_p = p;
    this->_box = NULL;
}

LightGrid::LightGrid(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * p, uint8_t rows, uint8_t columns) {
    this->rows = rows;
    this->columns = columns;
    this->_p = p;
    this->_box = NULL;
}

void LightGrid::_setColor() {
    for (uint8_t i = 0, n = this->rows * this->columns * 3; i < n; i+=3) {
        _p->SetPixelColor(i, RgbColor(_box[i], _box[i+1], _box[i+2]));
    }
}

void LightGrid::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = 0, n = rows * columns; i < n; i++) {
        _p->SetPixelColor(i, RgbColor(red, green, blue));
    }
}

void LightGrid::setColor(uint8_t number, uint8_t red, uint8_t green, uint8_t blue) {
    _p->SetPixelColor(number - 1, RgbColor(red, green, blue));
}

void LightGrid::setColor(uint8_t row, uint8_t column, uint8_t red, uint8_t green, uint8_t blue) {
    _p->SetPixelColor(0, RgbColor(red, green, blue));
}

void LightGrid::setColor(uint8_t box[], uint8_t rows, uint8_t columns) {
    this->rows = rows;
    this->columns = columns;
    if (this->_box) {
        delete []_box;
    }
    this->_box = box;
}


uint8_t* LightGrid::getColor(uint8_t row, uint8_t column) {
    uint8_t color[] = { _box[row * 3], _box[row * 3 + 1], _box[row * 3 + 2] };
    return color;
}
