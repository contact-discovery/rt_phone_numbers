//
// Created by Sebastian Schindler on 20.06.19.
//

#include "PrefixParser.h"
#include "csv.h"
#include <list>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

vector<tuple<string, int>> PrefixParser::read_numberspecs(string file) {
    vector<tuple<string, int>> out;
    io::CSVReader<3> in(file);
    in.read_header(io::ignore_extra_column, "Country", "Range", "Type");
    string Range; string country; string type;
    while(in.read_row(country, Range, type)){
        if (type == "proper") {
            continue;
        }
        string numberspec = Range;
        if (numberspec.front() == '+') numberspec.erase(0, 1);
        int extension_length = 0;
        string prefix = "";
        bool add_every = false;
        for ( int i = 0 ; i < numberspec.length(); i++)
        {
            if (isdigit(numberspec[i])) {
                prefix.push_back(numberspec[i]);
                continue;
            }
            if (numberspec[i] == 'X') {
                extension_length++;
                if (add_every || i + 1 == numberspec.length()) {
                    out.emplace_back(make_tuple(prefix, extension_length));
                }
                continue;
            }
            if (numberspec[i] == '(') {
                add_every = true;
                out.emplace_back(make_tuple(prefix, extension_length));
            }
        }
    }
    return out;
}

/*
bool PrefixParser::startsWith(string mainStr, string toMatch)
{
    // std::string::find returns 0 if toMatch is found at starting
    if(mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}


map<string, vector<string>> PrefixParser::country_prefixes() {
    map<string, vector<string>> map;
    map["US"] = {"1"};
    map["CA"] = {"1"};
    map["AG"] = {"1", "1268"};
    map["AI"] = {"1", "1264"};
    map["AS"] = {"1", "1684"};
    map["BB"] = {"1", "1246"};
    map["BM"] = {"1", "1441"};
    map["BS"] = {"1", "1242"};
    map["DM"] = {"1", "1767"};
    map["DO"] = {"1", "1809", "1829", "1849"};
    map["GD"] = {"1", "1473"};
    map["GU"] = {"1", "1671"};
    map["JM"] = {"1", "1658", "1876"};
    map["KN"] = {"1", "1869"};
    map["KY"] = {"1", "1345"};
    map["LC"] = {"1", "1758"};
    map["MP"] = {"1", "1670"};
    map["MS"] = {"1", "1664"};
    map["PR"] = {"1", "1787", "1939"};
    map["SX"] = {"1", "1721"};
    map["TC"] = {"1", "1649"};
    map["TT"] = {"1", "1868"};
    map["VC"] = {"1", "1784"};
    map["VG"] = {"1", "1284"};
    map["VI"] = {"1", "1340"};
    map["UM"] = {"1"};
    map["EG"] = {"20"};
    map["SS"] = {"211"};
    map["MA"] = {"212"};
    map["EH"] = {"212"};
    map["DZ"] = {"213"};
    map["TN"] = {"216"};
    map["LY"] = {"218"};
    map["GM"] = {"220"};
    map["SN"] = {"221"};
    map["MR"] = {"222"};
    map["ML"] = {"223"};
    map["GN"] = {"224"};
    map["CI"] = {"225"};
    map["BF"] = {"226"};
    map["NE"] = {"227"};
    map["TG"] = {"228"};
    map["BJ"] = {"229"};
    map["MU"] = {"230"};
    map["LR"] = {"231"};
    map["SL"] = {"232"};
    map["GH"] = {"233"};
    map["NG"] = {"234"};
    map["TD"] = {"235"};
    map["CF"] = {"236"};
    map["CM"] = {"237"};
    map["CV"] = {"238"};
    map["ST"] = {"239"};
    map["GQ"] = {"240"};
    map["GA"] = {"241"};
    map["CG"] = {"242"};
    map["CD"] = {"243"};
    map["AO"] = {"244"};
    map["GW"] = {"245"};
    map["IO"] = {"246"};
    map["AC"] = {"247"};
    map["SC"] = {"248"};
    map["SD"] = {"249"};
    map["RW"] = {"250"};
    map["ET"] = {"251"};
    map["SO"] = {"252"};
    map["DJ"] = {"253"};
    map["KE"] = {"254"};
    map["TZ"] = {"255"};
    map["UG"] = {"256"};
    map["BI"] = {"257"};
    map["MZ"] = {"258"};
    map["ZM"] = {"260"};
    map["MG"] = {"261"};
    map["RE"] = {"262"};
    map["YT"] = {"262"};
    map["TF"] = {"262"};
    map["ZW"] = {"263"};
    map["NA"] = {"264"};
    map["MW"] = {"265"};
    map["LS"] = {"266"};
    map["BW"] = {"267"};
    map["SZ"] = {"268"};
    map["KM"] = {"269"};
    map["ZA"] = {"27"};
    map["SH"] = {"290"};
    map["TA"] = {"290"};
    map["ER"] = {"291"};
    map["AW"] = {"297"};
    map["FO"] = {"298"};
    map["GL"] = {"299"};
    map["GR"] = {"30"};
    map["NL"] = {"31"};
    map["BE"] = {"32"};
    map["FR"] = {"33"};
    map["ES"] = {"34"};
    map["GI"] = {"350"};
    map["PT"] = {"351"};
    map["LU"] = {"352"};
    map["IE"] = {"353"};
    map["IS"] = {"354"};
    map["AL"] = {"355"};
    map["MT"] = {"356"};
    map["CY"] = {"357"};
    map["FI"] = {"358"};
    map["AX"] = {"358"};
    map["BG"] = {"359"};
    map["HU"] = {"36"};
    map["LT"] = {"370"};
    map["LV"] = {"371"};
    map["EE"] = {"372"};
    map["MD"] = {"373"};
    map["AM"] = {"374"};
    map["QN"] = {"374"};
    map["BY"] = {"375"};
    map["AD"] = {"376"};
    map["MC"] = {"377"};
    map["SM"] = {"378"};
    map["VA"] = {"379"};
    map["UA"] = {"380"};
    map["RS"] = {"381"};
    map["ME"] = {"382"};
    map["XK"] = {"383"};
    map["HR"] = {"385"};
    map["SO"] = {"386"};
    map["BA"] = {"387"};
    map["EU"] = {"388"};
    map["MK"] = {"389"};
    map["IT"] = {"39"};
    map["VA"] = {"39"};
    map["RO"] = {"40"};
    map["CH"] = {"41"};
    map["CZ"] = {"420"};
    map["SK"] = {"421"};
    map["LI"] = {"423"};
    map["AT"] = {"43"};
    map["UK"] = {"44"};
    map["GG"] = {"44"};
    map["IM"] = {"44"};
    map["JE"] = {"44"};
    map["DK"] = {"45"};
    map["SE"] = {"46"};
    map["NO"] = {"47"};
    map["SJ"] = {"47"};
    map["BV"] = {"47"};
    map["PL"] = {"48"};
    map["DE"] = {"49"};
    map["FK"] = {"500"};
    map["GS"] = {"500"};
    map["BZ"] = {"501"};
    map["GT"] = {"502"};
    map["SV"] = {"503"};
    map["HN"] = {"504"};
    map["NI"] = {"505"};
    map["CR"] = {"506"};
    map["PA"] = {"507"};
    map["PM"] = {"508"};
    map["HT"] = {"509"};
    map["PE"] = {"51"};
    map["MX"] = {"52"};
    map["CU"] = {"53"};
    map["AR"] = {"54"};
    map["BR"] = {"55"};
    map["CL"] = {"56"};
    map["CO"] = {"57"};
    map["VE"] = {"58"};
    map["GP"] = {"590"};
    map["BL"] = {"590"};
    map["MF"] = {"590"};
    map["BO"] = {"591"};
    map["GY"] = {"592"};
    map["EC"] = {"593"};
    map["GF"] = {"594"};
    map["PY"] = {"595"};
    map["MQ"] = {"596"};
    map["SR"] = {"597"};
    map["UY"] = {"598"};
    map["BQ"] = {"599"};
    map["CW"] = {"599"};
    map["MY"] = {"60"};
    map["AU"] = {"61"};
    map["CX"] = {"61"};
    map["CC"] = {"61"};
    map["ID"] = {"62"};
    map["PH"] = {"63"};
    map["NZ"] = {"64"};
    map["PN"] = {"64"};
    map["SG"] = {"65"};
    map["TH"] = {"66"};
    map["TL"] = {"670"};
    map["NF"] = {"672"};
    map["AQ"] = {"672"};
    map["HM"] = {"672"};
    map["BN"] = {"673"};
    map["NR"] = {"674"};
    map["PG"] = {"675"};
    map["TO"] = {"676"};
    map["SB"] = {"677"};
    map["VU"] = {"678"};
    map["FJ"] = {"679"};
    map["PW"] = {"680"};
    map["WF"] = {"681"};
    map["CK"] = {"682"};
    map["NU"] = {"683"};
    map["WS"] = {"685"};
    map["KI"] = {"686"};
    map["NC"] = {"687"};
    map["TV"] = {"688"};
    map["PF"] = {"689"};
    map["TK"] = {"690"};
    map["FM"] = {"691"};
    map["MH"] = {"692"};
    map["RU"] = {"7", "73", "74", "78", "79"};
    map["KZ"] = {"76", "77"};
    map["XT"] = {"800"};
    map["XS"] = {"808"};
    map["JP"] = {"81"};
    map["KR"] = {"82"};
    map["VN"] = {"84"};
    map["KP"] = {"850"};
    map["HK"] = {"852"};
    map["MO"] = {"853"};
    map["KH"] = {"855"};
    map["LA"] = {"856"};
    map["CN"] = {"86"};
    map["XN"] = {"870"};
    map["XP"] = {"878"};
    map["BD"] = {"880"};
    map["XG"] = {"881"};
    map["XV"] = {"882", "883"};
    map["TW"] = {"886"};
    map["XD"] = {"888"};
    map["TR"] = {"90"};
    map["CT"] = {"90"};
    map["IN"] = {"91"};
    map["PK"] = {"92"};
    map["AF"] = {"93"};
    map["LK"] = {"94"};
    map["MM"] = {"95"};
    map["MV"] = {"960"};
    map["LB"] = {"961"};
    map["JO"] = {"962"};
    map["SY"] = {"963"};
    map["IQ"] = {"964"};
    map["KW"] = {"965"};
    map["SA"] = {"966"};
    map["YE"] = {"967"};
    map["OM"] = {"968"};
    map["PS"] = {"970"};
    map["AE"] = {"971"};
    map["IL"] = {"972"};
    map["BH"] = {"973"};
    map["QA"] = {"974"};
    map["BT"] = {"975"};
    map["MN"] = {"976"};
    map["NP"] = {"977"};
    map["XR"] = {"979"};
    map["IR"] = {"98"};
    map["XC"] = {"991"};
    map["TJ"] = {"991"};
    map["TM"] = {"991"};
    map["AZ"] = {"991"};
    map["GE"] = {"991"};
    map["KG"] = {"991"};
    map["UZ"] = {"991"};
    return map;
}
*/