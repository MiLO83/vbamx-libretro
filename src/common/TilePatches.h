#include <string>
#include <list>

class Tile {
    uint32_t top_left_address;
    bool is256;
  public:
    void set_values (uint32_t, bool);
};

void Tile::set_values (uint32_t t_l_address, bool is8bpp) {
  top_left_address = t_l_address;
  is256 = is8bpp;
}

std::list<Tile> tileDB;
std::list<Tile>::iterator tileDBit;