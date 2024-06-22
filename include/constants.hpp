#pragma once

const int PORT = 9012;
const double TICK_RATE = 60.0f;
const double TICK_TIME = 1.0f / TICK_RATE;
const int BLOCK_HEIGHT = 4;
const int WIDTH = 10;
const int BOARD_HEIGHT = 20;


const std::string I = "\
XXXXIXXXXX\n\
XXXXIXXXXX\n\
XXXXIXXXXX\n\
XXXXIXXXXX\n\
";

const std::string L = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXLXXXXX\n\
XXXXLLLXXX\n\
";

const std::string J = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXXJXXX\n\
XXXXJJJXXX\n\
";

const std::string O = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXOOXXXX\n\
XXXXOOXXXX\n\
";


const std::string S = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXSSXXX\n\
XXXXSSXXXX\n\
";

const std::string T = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXTXXXX\n\
XXXXTTTXXX\n\
";

const std::string Z = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXZZXXXX\n\
XXXXXZZXXX\n\
";
