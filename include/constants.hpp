#pragma once

const int PORT = 9029;
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

const std::string I1 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXIIIIXXX\n\
";

const std::string J = "\
XXXXXXXXXX\n\
XXXXXJXXXX\n\
XXXXXJXXXX\n\
XXXXJJXXXX\n\
";

const std::string J1 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXJXXXXXX\n\
XXXJJJXXXX\n\
";

const std::string J2 = "\
XXXXXXXXXX\n\
XXXXJJXXXX\n\
XXXXJXXXXX\n\
XXXXJXXXXX\n\
";

const std::string J3 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXJJJXXX\n\
XXXXXXJXXX\n\
";

const std::string L = "\
XXXXXXXXXX\n\
XXXXLXXXXX\n\
XXXXLXXXXX\n\
XXXXLLXXXX\n\
";

const std::string L1 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXLLLXXX\n\
XXXXLXXXXX\n\
";

const std::string L2 = "\
XXXXXXXXXX\n\
XXXXXLLXXX\n\
XXXXXXLXXX\n\
XXXXXXLXXX\n\
";

const std::string L3 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXXLXXX\n\
XXXXLLLXXX\n\
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

const std::string S1 = "\
XXXXXXXXXX\n\
XXXXXSXXXX\n\
XXXXXSSXXX\n\
XXXXXXSXXX\n\
";

const std::string T = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXXTXXXX\n\
XXXXTTTXXX\n\
";

const std::string T1 = "\
XXXXXXXXXX\n\
XXXXXTXXXX\n\
XXXXXTTXXX\n\
XXXXXTXXXX\n\
";

const std::string T2 = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXTTTXXX\n\
XXXXXTXXXX\n\
";

const std::string T3 = "\
XXXXXXXXXX\n\
XXXXXTXXXX\n\
XXXXTTXXXX\n\
XXXXXTXXXX\n\
";

const std::string Z = "\
XXXXXXXXXX\n\
XXXXXXXXXX\n\
XXXXZZXXXX\n\
XXXXXZZXXX\n\
";

const std::string Z1 = "\
XXXXXXXXXX\n\
XXXXXZXXXX\n\
XXXXZZXXXX\n\
XXXXZXXXXX\n\
";

const std::string Is[] = {I, I1, I, I1};
const std::string Js[] = {J, J1, J2, J3};
const std::string Ls[] = {L, L1, L2, L3};
const std::string Os[] = {O, O, O, O};
const std::string Ss[] = {S, S1, S, S1};
const std::string Ts[] = {T, T1, T2, T3};
const std::string Zs[] = {Z, Z1, Z, Z1};