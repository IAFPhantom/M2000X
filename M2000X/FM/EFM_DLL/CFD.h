#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Log.h"
#include "aero.h"
#include "pugixml.hpp"

class  CFD   // load from FM config file
{
pugi::xml_document doc;


    double _CD = 0.0;
    double _CY = 0.0;
    double _CYp = 0.0;
    double _CYr = 0.0;
    double _CL = 0.0;
    double _Cl = 0.0;
    double _Clp = 0.0;
    double _Clr = 0.0;
    double _Cm = 0.0;
    double _Cmq = 0.0;
    double _Cn = 0.0;
    double _Cnp = 0.0;
    double _Cnr = 0.0;
    const std::string WHITESPACE = " \t\n";
    const std::string NUMERIC = "+-1234567890.";
    struct x_y_arry {
        std::vector<double> x_range;
        std::vector<double> y_range;
        std::vector<std::vector<double>> values;
    };
    struct x_arry {
        std::vector<double> x_range;
        std::vector<double> values;
    };
    //////////////////////////////////////////////////////////////// 
    double _round(const double& var, int digits = 1)
    {
        // 37.66666 * 100 =3766.66
        // 3766.66 + .5 =3767.16    for rounding off value
        // then type cast to int so value is 3767
        // then divided by 100 so the value converted into 37.67
        digits = (int)pow(10.0, digits);
        double value = 0;
        value = (double)round(var * (double)digits);
        return (double)value / digits;
    }
    bool ParseLine(const std::string& sLine, std::vector<double>& vals, const std::string& sDelimiters)
    {
        double val = 0;
        size_t last = 0;
        size_t found = sLine.find_first_of(sDelimiters); //",\t"
        while (found != std::string::npos)
        {
            std::string s = sLine.substr(last, found - last);
            last = found + 1;
            val = atof(s.c_str());
            vals.push_back(val);
            found = sLine.find_first_of(sDelimiters, last);
        }
        if (last != 0)
        {
            std::string s = sLine.substr(last, found - last);
            //			last = found + 1;
            val = atof(s.c_str());
            vals.push_back(val);
        }
        return !vals.empty();
    }
    bool parse2RowColumn(std::string values, x_y_arry& data)
    {
        std::vector<double> vals;
        std::string line;
        size_t last = values.find('\n',0);
        size_t next = values.find('\n', last+1);
        line = values.substr(last, next- last);
        
        splitLine(line, data.x_range);
        while (!line.empty())
        {
            last = next;
            next = values.find('\n', last+1);
            if (next == std::string::npos)
                next = values.size();
            line = values.substr(last, next- last);
            if (splitLine(line, vals))
            {
                data.y_range.push_back(vals[0]);
                vals.erase(vals.begin());
                data.values.push_back(vals);
            }
        }
        return true;
    }
    bool splitLine(std::string values, std::vector<double>& data)
    {
        data.clear();
        double val;
        size_t last = 0;
        size_t next = 0;
        while ((last = values.find_first_of(NUMERIC, last)) != std::string::npos)
        {
            if ((next = values.find_first_of(WHITESPACE, last)) == std::string::npos)
                return false;

            data.push_back( atof(values.substr(last, next - last).c_str()) );
            last = next + 1;
        }
        return data.size() != 0;
    }
    bool parse1Row(std::string values, x_arry& data)
    {
        data.x_range.clear();
        data.values.clear();
        double x;
        double val;
        size_t last = 0;
        size_t next = 0; 
        while ((last = values.find_first_of(NUMERIC, last)) != std::string::npos)
        { 
            if( (next = values.find_first_of(WHITESPACE, last)) == std::string::npos)
                return false;

            x = atof(values.substr(last, next - last).c_str());
            last = next + 1;

            if ((next = values.find_first_of(NUMERIC, last)) == std::string::npos)
                return false; // error : odd number of values
            last = next ;
            if ((next = values.find_first_of(WHITESPACE, last)) == std::string::npos)
                next = values.size();
            val = atof(values.substr(last, next - last).c_str());
            last = next + 1;

            data.x_range.push_back(x);
            data.values.push_back(val);
        }
        if(data.x_range.size() ==0)
            return false;
        return true;
    };
    bool loadFunction(std::string sFunction, std::vector<std::string> & data)
    {
        data.clear();
        pugi::xml_node tools = doc.child("fdm_config").child("aerodynamics");
        //for (pugi::xml_node tool : doc.child("fdm_config").children("aerodynamics"))
        for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
        {
            if (sFunction.compare(tool.first_attribute().value()) == 0)
            {
                pugi::xml_node table;
                pugi::xml_node t = tool.child("table").child("tableData").first_child();
                if (t == NULL)
                    table = tool.child("product").child("table");
                else
                    table = tool.child("table");

                for (pugi::xml_node child : table.children())
                //for (pugi::xml_node child : tool.child("table").children())
                {
                    if (std::string(child.name()).compare("tableData") == 0)
                    {
                        data.push_back(child.first_child().value());
                    }
                    /*pugi::xml_node table;
                    pugi::xml_node t = tool.child("table").child("tableData").first_child();
                    if (t == NULL)
                        table = tool.child("product").child("table").child("tableData").first_child();
                    else
                        table = tool.child("table").child("tableData").first_child();
                    for (; table; table = table.next_sibling())
                    {
                        data.push_back(table.value());
                    }*/
                }
                return !data.empty();
            }
        }
        return false;
    }
    double linearInterpolation(const double& y0, const double& y1, const double& x0, const double& x1, const double& xp)
    {
        return y0 + ((y1 - y0) / (x1 - x0)) * (xp - x0);
    }
    double BilinearInterpolation(const double& q11, const double& q12, const double& q21, const double& q22, const double& x1, const double& x2, const double& y1, const double& y2, const double& x, const double& y)
    {
        double R1 = linearInterpolation(q11, q21, x1, x2, x);
        double R2 = linearInterpolation(q12, q22, x1, x2, x);
        double  P = linearInterpolation(R1, R2, y1, y2, y);
        return P;
    }
    size_t findIndex(double xp, const std::vector<double>& data)
    {
        if (data.size() == 0)
            return -1;
        if (data.size() == 1 || xp <= data.front() )
            return 0;
        if (xp >= data.back())
            return data.size() - 2;
        for (size_t i = 0; i < data.size()-1; i++)
            if (xp >= data[i] && xp <= data[i+1])
                return i;
        return -1;
    }
    double getValue(double xp, const x_arry& data)
    {
        size_t i = findIndex(xp, data.x_range);
        if(data.x_range.size()>1 && data.values.size() > 1 && i != -1)
            return linearInterpolation(data.values[i], data.values[i+1], data.x_range[i], data.x_range[i+1], xp);
        return 0.0;
    }
    double get2Value(double xp, double yp, const x_y_arry& data)
    {
        if (data.x_range.size() == 0 || data.y_range.size() == 0 )// error no lines
            return 0;
        if (data.x_range.size() == 1 || data.y_range.size() == 1) // error no lines
            return 0.0;
        // find column
        size_t x1 = findIndex(xp, data.x_range);
        size_t x2 = x1+1;
        // find row
        size_t y1 = findIndex(yp, data.y_range);;
        size_t y2 = y1+1;
        // bilinear interpolation
        if (data.x_range.size() > 1 && data.y_range.size() > 1 && data.values.size() > 1 && x1 != -1 && y1 != -1)
            return BilinearInterpolation(data.values[y1][x1], data.values[y1][x2], data.values[y2][x1], data.values[y2][x2], data.x_range[x1], data.x_range[x2], data.y_range[y1], data.y_range[y2],xp, yp);
        return 0.0;
    }
/// DRAG ///////////////////////////////////////////////////////////// 
    x_arry CFXB = { };              //BASIC DRAG
//    x_y_arry CFXCTNK = { };         //DRAG INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CFXDED1L_16 = {};      //DRAG DUE TO ELEVON 1L
    x_y_arry CFXDED1L_0 = {};
    x_y_arry CFXDED1L_25 = {};
    x_y_arry CFXDED1R_16 = {};      //DRAG DUE TO ELEVON 1R
    x_y_arry CFXDED1R_0 = {};
    x_y_arry CFXDED1R_25 = {};
    x_y_arry CFXDED2L_16 = {};      //DRAG DUE TO ELEVON 2L
    x_y_arry CFXDED2L_0 = {};
    x_y_arry CFXDED2L_25 = {};
    x_y_arry CFXDED2R_16 = {};      //DRAG DUE TO ELEVON 2R
    x_y_arry CFXDED2R_0 = {};
    x_y_arry CFXDED2R_25 = {};
    x_arry CFXDSBL = { };           //DRAG DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CFXDSBU = { };           //DRAG DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CFXDSD1L = { };          //DRAG DUE TO LE SLAT 1
    x_arry CFXDSD2L = { };          //DRAG DUE TO LE SLAT 2
    x_arry CFXGEAR = { };           //DRAG INCREMENT DUE TO GEAR
    x_y_arry CFXmn = { };             //DRAG DUE TO MACHD
////LIFT //////////////////////////////////////////////////////////// 
    x_arry CFZB = { };              //BASIC LIFT
//    x_y_arry CFZCTNK = { };         //LIFT INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CFZDED1L_16 = {};      //LIFT DUE TO ELEVON 1L
    x_y_arry CFZDED1L_0 = {};
    x_y_arry CFZDED1L_25 = {};
    x_y_arry CFZDED1R_16 = {};      //LIFT DUE TO ELEVON 1R
    x_y_arry CFZDED1R_0 = {};
    x_y_arry CFZDED1R_25 = {};
    x_y_arry CFZDED2L_16 = {};      //LIFT DUE TO ELEVON 2L
    x_y_arry CFZDED2L_0 = {};
    x_y_arry CFZDED2L_25 = {};
    x_y_arry CFZDED2R_16 = {};      //LIFT DUE TO ELEVON 2R
    x_y_arry CFZDED2R_0 = {};
    x_y_arry CFZDED2R_25 = {};
    x_arry CFZDSBL = { };           //LIFT DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CFZDSBU = { };           //LIFT DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CFZDSD1L = { };          //LIFT DUE TO LE SLAT 1
    x_arry CFZDSD2L = { };          //LIFT DUE TO LE SLAT 2
    x_arry CFZGEAR = { };           //LIFT INCREMENT DUE TO GEAR
    x_y_arry CFZmn = { };             //LIFT DUE TO MACH
//// SIDEFORCE //////////////////////////////////////////////////////////// 
    x_y_arry CFYB = { };              //BASIC SIDE FORCE
//    x_y_arry CFYCTNK = { };         //SIDE FORCE INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CFYDED1L_16 = {};      //SIDE FORCE DUE TO ELEVON 1L
    x_y_arry CFYDED1L_0 = {};
    x_y_arry CFYDED1L_25 = {};
    x_y_arry CFYDED1R_16 = {};      //SIDE FORCE DUE TO ELEVON 1R
    x_y_arry CFYDED1R_0 = {};
    x_y_arry CFYDED1R_25 = {};
    x_y_arry CFYDED2L_16 = {};      //SIDE FORCE DUE TO ELEVON 2L
    x_y_arry CFYDED2L_0 = {};
    x_y_arry CFYDED2L_25 = {};
    x_y_arry CFYDED2R_16 = {};      //SIDE FORCE DUE TO ELEVON 2R
    x_y_arry CFYDED2R_0 = {};
    x_y_arry CFYDED2R_25 = {};
    x_arry CFYDSBL = { };           //SIDE FORCE DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CFYDSBU = { };           //SIDE FORCE DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CFYDSD1L = { };          //SIDE FORCE DUE TO LE SLAT 1
    x_arry CFYDSD2L = { };          //SIDE FORCE DUE TO LE SLAT 2
    x_arry CFYGEAR = { };           //SIDE FORCE INCREMENT DUE TO GEAR
    x_arry CFYP = { };             //SIDE FORCE DUE TO ROLL RATE
    x_arry CFYR = { };             //SIDE FORCE DUE TO YAW RATE
//// PITCHING MOMENT //////////////////////////////////////////////////////////// 
    x_arry CMM1 = { };              //BASIC PITCHING MOMENT
//    x_y_arry CMMCTNK = { };         //PITCHING MOMENT INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CMMDED1L_16 = {};      //PITCH MOMENT  DUE TO ELEVON 1L
    x_y_arry CMMDED1L_0 = {};
    x_y_arry CMMDED1L_25 = {};
    x_y_arry CMMDED1R_16 = {};      //PITCH MOMENT  DUE TO ELEVON 1R
    x_y_arry CMMDED1R_0 = {};
    x_y_arry CMMDED1R_25 = {};
    x_y_arry CMMDED2L_16 = {};      //PITCH MOMENT  DUE TO ELEVON 2L
    x_y_arry CMMDED2L_0 = {};
    x_y_arry CMMDED2L_25 = {};
    x_y_arry CMMDED2R_16 = {};      //PITCH MOMENT  DUE TO ELEVON 2R
    x_y_arry CMMDED2R_0 = {};
    x_y_arry CMMDED2R_25 = {};
    x_arry CMMDSBL = { };           //PITCH MOMENT  DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CMMDSBU = { };           //PITCH MOMENT  DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CMMDSD1L = { };          //PITCH MOMENT  DUE TO LE SLAT 1
    x_arry CMMDSD2L = { };          //PITCH MOMENT  DUE TO LE SLAT 2
    x_arry CMMGEAR = { };           //PITCH MOMENT  INCREMENT DUE TO GEAR
    x_y_arry CMMmnw = { };          //PITCH MOMENT  DUE TO MACH
    x_arry CMMQ = {};               // PITCH DAMPING DERIVATIVE
    ///ROLLING MOMENT ///////////////////////////////////////////////////////////// 
    x_y_arry CML1 = { };              //BASIC ROLLING MOMENT
//    x_y_arry CMLCTNK = { };         //ROLLING MOMENT INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CMLDED1L_16 = {};      //ROLLING MOMENT  DUE TO ELEVON 1L
    x_y_arry CMLDED1L_0 = {};
    x_y_arry CMLDED1L_25 = {};
    x_y_arry CMLDED1R_16 = {};      //ROLLING MOMENT  DUE TO ELEVON 1R
    x_y_arry CMLDED1R_0 = {};
    x_y_arry CMLDED1R_25 = {};
    x_y_arry CMLDED2L_16 = {};      //ROLLING MOMENT  DUE TO ELEVON 2L
    x_y_arry CMLDED2L_0 = {};
    x_y_arry CMLDED2L_25 = {};
    x_y_arry CMLDED2R_16 = {};      //ROLLING MOMENT  DUE TO ELEVON 2R
    x_y_arry CMLDED2R_0 = {};
    x_y_arry CMLDED2R_25 = {};
    x_arry CMLDSBL = { };           //ROLLING MOMENT  DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CMLDSBU = { };           //ROLLING MOMENT  DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CMLDSD1L = { };          //ROLLING MOMENT  DUE TO LE SLAT 1
    x_arry CMLDSD2L = { };          //ROLLING MOMENT  DUE TO LE SLAT 2
    x_arry CMLGEAR = { };           //ROLLING MOMENT  INCREMENT DUE TO GEAR
    x_y_arry CMLmnw = { };          //ROLLING MOMENT  DUE TO MACH
    x_arry CMLP = {};               // ROLL DAMPING DERIVATIVE
    x_arry CMLR = {};               // ROLLING MOMENT DUE TO YAW RATE
    //////////////////////////////////////////////////////////////// 
    x_y_arry CMN1 = { };              //BASIC YAWING MOMENT
//    x_y_arry CMMCTNK = { };         //YAWING MOMENT INCREMENT DUE TO TANK(CENTRE)
    x_y_arry CMNDED1L_16 = {};      //YAWING MOMENT  DUE TO ELEVON 1L
    x_y_arry CMNDED1L_0 = {};
    x_y_arry CMNDED1L_25 = {};
    x_y_arry CMNDED1R_16 = {};      //YAWING MOMENT  DUE TO ELEVON 1R
    x_y_arry CMNDED1R_0 = {};
    x_y_arry CMNDED1R_25 = {};
    x_y_arry CMNDED2L_16 = {};      //YAWING MOMENT  DUE TO ELEVON 2L
    x_y_arry CMNDED2L_0 = {};
    x_y_arry CMNDED2L_25 = {};
    x_y_arry CMNDED2R_16 = {};      //YAWING MOMENT  DUE TO ELEVON 2R
    x_y_arry CMNDED2R_0 = {};
    x_y_arry CMNDED2R_25 = {};
    x_y_arry CMNDRDr_N25 = {};      //YAWING MOMENT DUE TO RUDDER DEFLECTION
    x_y_arry CMNDRDr_0 = {};
    x_y_arry CMNDRDr_25 = {};
    x_arry CMNDSBL = { };           //YAWING MOMENT  DUE TO LOWER SPEEDBRAKE DEFLECTION
    x_arry CMNDSBU = { };           //YAWING MOMENT  DUE TO UPPER SPEEDBRAKE DEFLECTION
    x_arry CMNDSD1L = { };          //YAWING MOMENT  DUE TO LE SLAT 1
    x_arry CMNDSD2L = { };          //YAWING MOMENT  DUE TO LE SLAT 2
    x_arry CMNGEAR = { };           //YAWING MOMENT  INCREMENT DUE TO GEAR
    x_arry CMNP = {};               // YAWING MOMENT DUE TO ROLL RATE
    x_arry CMNR = {};               // YAW DAMPING DERIVATIVE
    //////////////////////////////////////////////////////////////// 
public:
    bool load(std::string sFMFilePath)
    {
        pugi::xml_parse_result result = doc.load_file(sFMFilePath.c_str());
        if (!result)
        {
            CLog::trace(1, std::string("failed loading: " + sFMFilePath + "\n").c_str());
            return false;
        }
        else
        {
            CLog::trace(1, std::string("loaded: " + sFMFilePath + "\n").c_str());
            CLog::cotrace("CFD,CD,_CFXB, _CFXDED1L, _CFXDED1R, _CFXDED2L, _CFXDED2R, _CFXDSBL, _CFXDSBU, _CFXDSD1L, _CFXDSD2L, _CFXGEAR, _CFXmn\n");
            CLog::cotrace("CFL,CL,_CFZB, _CFZDED1L, _CFZDED1R, _CFZDED2L, _CFZDED2R, _CFZDSBL, _CFZDSBU, _CFZDSD1L, _CFZDSD2L, _CFZGEAR\n");
            CLog::cotrace("CFY,CY,_CFYB, _CFYDED1L, _CFYDED1R, _CFYDED2L, _CFYDED2R, _CFYDSBL, _CFYDSBU, _CFYDSD1L, _CFYDSD2L, _CFYGEAR, CFYP,CFYR\n");
            CLog::cotrace("CML,CL,_CML1, _CMLDED1L, _CMLDED1R, _CMLDED2L, _CMLDED2R, _CMLDSBL, _CMLDSBU, _CMLDSD1L, _CMLDSD2L, _CMLGEAR, CMLP,CMLR\n");
            CLog::cotrace("CMM,CM,_CMM1, _CMMDED1L, _CMMDED1R, _CMMDED2L, _CMMDED2R, _CMMDSBL, _CMMDSBU, _CMMDSD1L, _CMMDSD2L, _CMMGEAR, CMMmnw, CMMQ\n");
            CLog::cotrace("CMN,CN,_CMN1, _CMNDED1L, _CMNDED1R, _CMNDED2L, _CMNDED2R, _CMNDSBL, _CMNDSBU, _CMNDSD1L, _CMNDSD2L, _CMNGEAR, CMNP,CMNR\n");
        }
        std::vector<std::string> data;

// drag
        if (CFD::loadFunction("aero/c/CFXB", data))
            parse1Row(data.front(), CFXB);
        if (CFD::loadFunction("aero/c/CFXDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFXDED1L_16);
                parse2RowColumn(data[1], CFXDED1L_0);
                parse2RowColumn(data[2], CFXDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFXDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFXDED1R_16);
                parse2RowColumn(data[1], CFXDED1R_0);
                parse2RowColumn(data[2], CFXDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFXDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFXDED2L_16);
                parse2RowColumn(data[1], CFXDED2L_0);
                parse2RowColumn(data[2], CFXDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFXDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFXDED2R_16);
                parse2RowColumn(data[1], CFXDED2R_0);
                parse2RowColumn(data[2], CFXDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFXDSBL", data))
            parse1Row(data.front(), CFXDSBL);
        if (CFD::loadFunction("aero/c/CFXDSBU", data))
            parse1Row(data.front(), CFXDSBU);
        if (CFD::loadFunction("aero/c/CFXDSD1L", data))
            parse1Row(data.front(), CFXDSD1L);
        if (CFD::loadFunction("aero/c/CFXDSD2L", data))
            parse1Row(data.front(), CFXDSD2L);
        if (CFD::loadFunction("aero/c/CFXGEAR", data))
            parse1Row(data.front(), CFXGEAR);
        if (CFD::loadFunction("aero/c/CFXmn", data))
            parse2RowColumn(data.front(), CFXmn);
        
// lift
        if (CFD::loadFunction("aero/c/CFZB", data))
            parse1Row(data.front(), CFZB);
        if (CFD::loadFunction("aero/c/CFZDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFZDED1L_16);
                parse2RowColumn(data[1], CFZDED1L_0);
                parse2RowColumn(data[2], CFZDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFZDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFZDED1R_16);
                parse2RowColumn(data[1], CFZDED1R_0);
                parse2RowColumn(data[2], CFZDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFZDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFZDED2L_16);
                parse2RowColumn(data[1], CFZDED2L_0);
                parse2RowColumn(data[2], CFZDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFZDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFZDED2R_16);
                parse2RowColumn(data[1], CFZDED2R_0);
                parse2RowColumn(data[2], CFZDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFZDSBU", data)) // using same data for lower as upper
            parse1Row(data.front(), CFZDSBL);
        if (CFD::loadFunction("aero/c/CFZDSBU", data))
            parse1Row(data.front(), CFZDSBU);
        if (CFD::loadFunction("aero/c/CFZDSD1L", data))
            parse1Row(data.front(), CFZDSD1L);
        if (CFD::loadFunction("aero/c/CFZDSD2L", data))
            parse1Row(data.front(), CFZDSD2L);
        if (CFD::loadFunction("aero/c/CFZGEAR", data))
            parse1Row(data.front(), CFZGEAR);
        if (CFD::loadFunction("aero/c/CFZmn", data))
            parse2RowColumn(data.front(), CFZmn);

// side force        
        if (CFD::loadFunction("aero/c/CFYB", data))
            parse2RowColumn(data.front(), CFYB);
        if (CFD::loadFunction("aero/c/CFYDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFYDED1L_16);
                parse2RowColumn(data[1], CFYDED1L_0);
                parse2RowColumn(data[2], CFYDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFYDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFYDED1R_16);
                parse2RowColumn(data[1], CFYDED1R_0);
                parse2RowColumn(data[2], CFYDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFYDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFYDED2L_16);
                parse2RowColumn(data[1], CFYDED2L_0);
                parse2RowColumn(data[2], CFYDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFYDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CFYDED2R_16);
                parse2RowColumn(data[1], CFYDED2R_0);
                parse2RowColumn(data[2], CFYDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CFYDSBL", data))
            parse1Row(data.front(), CFYDSBL);
        if (CFD::loadFunction("aero/c/CFYDSBU", data))
            parse1Row(data.front(), CFYDSBU);
        if (CFD::loadFunction("aero/c/CFYDSD1L", data))
            parse1Row(data.front(), CFYDSD1L);
        if (CFD::loadFunction("aero/c/CFYDSD2L", data))
            parse1Row(data.front(), CFYDSD2L);
        if (CFD::loadFunction("aero/c/CFYGEAR", data))
            parse1Row(data.front(), CFYGEAR);
        if (CFD::loadFunction("aero/c/CFYP", data))
            parse1Row(data.front(), CFYP);
        if (CFD::loadFunction("aero/c/CFYR", data))
            parse1Row(data.front(), CFYR);

// ROLLING moment        
        if (CFD::loadFunction("aero/c/CML1", data))
            parse2RowColumn(data.front(), CML1);
        if (CFD::loadFunction("aero/c/CMLDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMLDED1L_16);
                parse2RowColumn(data[1], CMLDED1L_0);
                parse2RowColumn(data[2], CMLDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMLDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMLDED1R_16);
                parse2RowColumn(data[1], CMLDED1R_0);
                parse2RowColumn(data[2], CMLDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMLDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMLDED2L_16);
                parse2RowColumn(data[1], CMLDED2L_0);
                parse2RowColumn(data[2], CMLDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMLDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMLDED2R_16);
                parse2RowColumn(data[1], CMLDED2R_0);
                parse2RowColumn(data[2], CMLDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMLDSBL", data))
            parse1Row(data.front(), CMLDSBL);
        if (CFD::loadFunction("aero/c/CMLDSBU", data))
            parse1Row(data.front(), CMLDSBU);
        if (CFD::loadFunction("aero/c/CMLDSD1L", data))
            parse1Row(data.front(), CMLDSD1L);
        if (CFD::loadFunction("aero/c/CMLDSD2L", data))
            parse1Row(data.front(), CMLDSD2L);
        if (CFD::loadFunction("aero/c/CMLGEAR", data))
            parse1Row(data.front(), CMLGEAR);
        if (CFD::loadFunction("aero/c/CMLmnw", data))
            parse2RowColumn(data.front(), CMLmnw);
        if (CFD::loadFunction("aero/c/CMLP", data))
            parse1Row(data.front(), CMLP);
        if (CFD::loadFunction("aero/c/CMLR", data))
            parse1Row(data.front(), CMLR);

// pitch moment        
        if (CFD::loadFunction("aero/c/CMM1", data))
            parse1Row(data.front(), CMM1);
        if (CFD::loadFunction("aero/c/CMMDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMMDED1L_16);
                parse2RowColumn(data[1], CMMDED1L_0);
                parse2RowColumn(data[2], CMMDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMMDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMMDED1R_16);
                parse2RowColumn(data[1], CMMDED1R_0);
                parse2RowColumn(data[2], CMMDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMMDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMMDED2L_16);
                parse2RowColumn(data[1], CMMDED2L_0);
                parse2RowColumn(data[2], CMMDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMMDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMMDED2R_16);
                parse2RowColumn(data[1], CMMDED2R_0);
                parse2RowColumn(data[2], CMMDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMMDSBL", data))
            parse1Row(data.front(), CMMDSBL);
        if (CFD::loadFunction("aero/c/CMMDSBU", data))
            parse1Row(data.front(), CMMDSBU);
        if (CFD::loadFunction("aero/c/CMMDSD1L", data))
            parse1Row(data.front(), CMMDSD1L);
        if (CFD::loadFunction("aero/c/CMMDSD2L", data))
            parse1Row(data.front(), CMMDSD2L);
        if (CFD::loadFunction("aero/c/CMMGEAR", data))
            parse1Row(data.front(), CMMGEAR);
        if (CFD::loadFunction("aero/c/CMMmnw", data))
            parse2RowColumn(data.front(), CMMmnw);
        if (CFD::loadFunction("aero/c/CMMQ", data))
            parse1Row(data.front(), CMMQ);


// YAWING moment        
        if (CFD::loadFunction("aero/c/CMN1", data))
            parse2RowColumn(data.front(), CMN1);
        if (CFD::loadFunction("aero/c/CMNDED1L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMNDED1L_16);
                parse2RowColumn(data[1], CMNDED1L_0);
                parse2RowColumn(data[2], CMNDED1L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMNDED1R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMNDED1R_16);
                parse2RowColumn(data[1], CMNDED1R_0);
                parse2RowColumn(data[2], CMNDED1R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMNDED2L", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMNDED2L_16);
                parse2RowColumn(data[1], CMNDED2L_0);
                parse2RowColumn(data[2], CMNDED2L_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMNDED2R", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMNDED2R_16);
                parse2RowColumn(data[1], CMNDED2R_0);
                parse2RowColumn(data[2], CMNDED2R_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMNDSBL", data))
            parse1Row(data.front(), CMNDSBL);
        if (CFD::loadFunction("aero/c/CMNDSBU", data))
            parse1Row(data.front(), CMNDSBU);
        if (CFD::loadFunction("aero/c/CMNDSD1L", data))
            parse1Row(data.front(), CMNDSD1L);
        if (CFD::loadFunction("aero/c/CMNDSD2L", data))
            parse1Row(data.front(), CMNDSD2L);
        if (CFD::loadFunction("aero/c/CMNGEAR", data))
            parse1Row(data.front(), CMNGEAR);
        if (CFD::loadFunction("aero/c/CMNDRDr", data))
        {
            if (data.size() == 3)
            {
                parse2RowColumn(data[0], CMNDRDr_N25);
                parse2RowColumn(data[1], CMNDRDr_0);
                parse2RowColumn(data[2], CMNDRDr_25);
            }
            else
                return false;
        }
        if (CFD::loadFunction("aero/c/CMNP", data))
            parse1Row(data.front(), CMNP);
        if (CFD::loadFunction("aero/c/CMNR", data))
            parse1Row(data.front(), CMNR);




        return true;
    }

    void calc_coeffeients(PlaneState & State)
    {
        double alpha_deg = State.alpha * 57.296;
        double beta_deg = State.beta * 57.296;
        //BASIC DRAG
        double _CFXB = getValue(alpha_deg, CFXB); 
        //DRAG DUE TO ELEVON 1L
        double _CFXDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CFXDED1L_0);
        if(State.elevon1L < 0)
            _CFXDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg,CFXDED1L_16), 0, -16, State.elevon1L);
        else
            _CFXDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED1L_25), 0, 25, State.elevon1L);

        //DRAG DUE TO ELEVON 1R
        double _CFXDED1R;
        if (State.elevon1R < 0)
            _CFXDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED1R_16), 0, -16, State.elevon1R);      //DRAG DUE TO ELEVON 1R
        else
            _CFXDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED1R_25), 0, 25, State.elevon1R);      //DRAG DUE TO ELEVON 1R

        //DRAG DUE TO ELEVON 2L
        double _CFXDED2L;
        if (State.elevon2L < 0)
            _CFXDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED2L_16), 0, -16, State.elevon2L);      //DRAG DUE TO ELEVON 2L
        else
            _CFXDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED2L_25), 0, 25, State.elevon2L);      //DRAG DUE TO ELEVON 2L

        //DRAG DUE TO ELEVON 2R
        double _CFXDED2R;
        if (State.elevon2R < 0)
            _CFXDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED2R_16), 0, -16, State.elevon2R);      //DRAG DUE TO ELEVON 2R
        else
            _CFXDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFXDED2R_25), 0, 25, State.elevon2R);      //DRAG DUE TO ELEVON 2R

        //DRAG DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CFXDSBL = getValue(alpha_deg, CFXDSBL) * State.airbrakes;
        //DRAG DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CFXDSBU = getValue(alpha_deg, CFXDSBU) * State.airbrakes;
        //DRAG DUE TO LE SLAT 1
        double _CFXDSD1L = getValue(alpha_deg, CFXDSD1L) * State.slats1;
        //DRAG DUE TO LE SLAT 2
        double _CFXDSD2L = getValue(alpha_deg, CFXDSD2L) * State.slats2;
        //DRAG INCREMENT DUE TO GEAR
        double _CFXGEAR = getValue(alpha_deg, CFXGEAR) * State.gear;
        //DRAG DUE TO MACH
        double _CFXmn = get2Value(alpha_deg, State.mach, CFXmn) ;

        _CD = _CFXB + _CFXDED1L + _CFXDED1R
            + _CFXDED2L + _CFXDED2R + _CFXDSBL + _CFXDSBU
            + _CFXDSD1L + _CFXDSD2L + _CFXGEAR + _CFXmn;
        CLog::cotrace("CFD,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _CD, _CFXB, _CFXDED1L, _CFXDED1R, _CFXDED2L, _CFXDED2R, _CFXDSBL, _CFXDSBU, _CFXDSD1L, _CFXDSD2L, _CFXGEAR, _CFXmn);
        //////////////////////////////////////////////////////////////////////
        //BASIC LIFT
        double _CFZB = getValue(alpha_deg, CFZB); 
        //LIFT DUE TO ELEVON 1L
        double _CFZDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CFZDED1L_0);
        if(State.elevon1L < 0)
            _CFZDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg,CFZDED1L_16), 0, -16, State.elevon1L);
        else
            _CFZDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED1L_25), 0, 25, State.elevon1L);

        //LIFT DUE TO ELEVON 1R
        double _CFZDED1R;
        if (State.elevon1R < 0)
            _CFZDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED1R_16), 0, -16, State.elevon1R);      //LIFT DUE TO ELEVON 1R
        else
            _CFZDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED1R_25), 0, 25, State.elevon1R);      //LIFT DUE TO ELEVON 1R

        //LIFT DUE TO ELEVON 2L
        double _CFZDED2L;
        if (State.elevon2L < 0)
            _CFZDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED2L_16), 0, -16, State.elevon2L);      //LIFT DUE TO ELEVON 2L
        else
            _CFZDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED2L_25), 0, 25, State.elevon2L);      //LIFT DUE TO ELEVON 2L

        //LIFT DUE TO ELEVON 2R
        double _CFZDED2R;
        if (State.elevon2R < 0)
            _CFZDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED2R_16), 0, -16, State.elevon2R);      //LIFT DUE TO ELEVON 2R
        else
            _CFZDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFZDED2R_25), 0, 25, State.elevon2R);      //LIFT DUE TO ELEVON 2R

        //LIFT DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CFZDSBL = getValue(alpha_deg, CFZDSBL) * State.airbrakes;
        //LIFT DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CFZDSBU = getValue(alpha_deg, CFZDSBU) * State.airbrakes;
        //LIFT DUE TO LE SLAT 1
        double _CFZDSD1L = getValue(alpha_deg, CFZDSD1L) * State.slats1;
        //LIFT DUE TO LE SLAT 2
        double _CFZDSD2L = getValue(alpha_deg, CFZDSD2L) * State.slats2;
        //LIFT INCREMENT DUE TO GEAR
        double _CFZGEAR = getValue(alpha_deg, CFZGEAR) * State.gear;
        //LIFT DUE TO MACH
        double _CFZmn = get2Value(alpha_deg, State.mach, CFZmn);

        _CL = _CFZB + _CFZDED1L + _CFZDED1R 
            + _CFZDED2L + _CFZDED2R  + _CFZDSBL + _CFZDSBU
            + _CFZDSD1L + _CFZDSD2L + _CFZGEAR + _CFZmn;
        CLog::cotrace("CFL,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _CL, _CFZB, _CFZDED1L, _CFZDED1R, _CFZDED2L, _CFZDED2R, _CFZDSBL, _CFZDSBU, _CFZDSD1L , _CFZDSD2L , _CFZGEAR , _CFZmn);

// PITCHING MOMENT
        double _CMM1 = getValue(alpha_deg, CMM1);
        //PITCH MOMENT DUE TO ELEVON 1L
        double _CMMDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CMMDED1L_0);
        if (State.elevon1L < 0)
            _CMMDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED1L_16), 0, -16, State.elevon1L);
        else
            _CMMDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED1L_25), 0, 25, State.elevon1L);

        //PITCH MOMENT DUE TO ELEVON 1R
        double _CMMDED1R;
        if (State.elevon1R < 0)
            _CMMDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED1R_16), 0, -16, State.elevon1R);      //DRAG DUE TO ELEVON 1R
        else
            _CMMDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED1R_25), 0, 25, State.elevon1R);      //DRAG DUE TO ELEVON 1R

        //PITCH MOMENT DUE TO ELEVON 2L
        double _CMMDED2L;
        if (State.elevon2L < 0)
            _CMMDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED2L_16), 0, -16, State.elevon2L);      //DRAG DUE TO ELEVON 2L
        else
            _CMMDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED2L_25), 0, 25, State.elevon2L);      //DRAG DUE TO ELEVON 2L

        //PITCH MOMENT DUE TO ELEVON 2R
        double _CMMDED2R;
        if (State.elevon2R < 0)
            _CMMDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED2R_16), 0, -16, State.elevon2R);      //DRAG DUE TO ELEVON 2R
        else
            _CMMDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMMDED2R_25), 0, 25, State.elevon2R);      //DRAG DUE TO ELEVON 2R

        //PITCH MOMENT DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CMMDSBL = getValue(alpha_deg, CMMDSBL) * State.airbrakes;
        //PITCH MOMENT DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CMMDSBU = getValue(alpha_deg, CMMDSBU) * State.airbrakes;
        //PITCH MOMENT DUE TO LE SLAT 1
        double _CMMDSD1L = getValue(alpha_deg, CMMDSD1L) * State.slats1;
        //PITCH MOMENT DUE TO LE SLAT 2
        double _CMMDSD2L = getValue(alpha_deg, CMMDSD2L) * State.slats2;
        //PITCH MOMENT INCREMENT DUE TO GEAR
        double _CMMGEAR = getValue(alpha_deg, CMMGEAR) * State.gear;
        //PITCH MOMENT DUE TO MACH
        double _CMMmnw = get2Value(alpha_deg, State.mach, CMMmnw);

        _Cmq = getValue(alpha_deg, CMMQ);
        _Cm = _CMM1 
            + _CMMDED1L + _CMMDED1R + _CMMDED2L + _CMMDED2R  
            + _CMMDSBL + _CMMDSBU
            + _CMMDSD1L + _CMMDSD2L + _CMMGEAR + _CMMmnw;
        CLog::cotrace("CMM,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _Cm, _CMM1, _CMMDED1L, _CMMDED1R, _CMMDED2L, _CMMDED2R, _CMMDSBL, _CMMDSBU, _CMMDSD1L, _CMMDSD2L, _CMMGEAR, _CMMmnw, _Cmq);
        


// SIDE FORCE
        //BASIC SIDE FORCE
        double _CFY1 = get2Value(alpha_deg, beta_deg, CFYB);
        //SIDE FORCE DUE TO ELEVON 1L
        double _CFYDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CFYDED1L_0);
        if (State.elevon1L < 0)
            _CFYDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED1L_16), 0, -16, State.elevon1L);
        else
            _CFYDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED1L_25), 0, 25, State.elevon1L);

        //SIDE FORCE DUE TO ELEVON 1R
        double _CFYDED1R;
        if (State.elevon1R < 0)
            _CFYDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED1R_16), 0, -16, State.elevon1R);      //DRAG DUE TO ELEVON 1R
        else
            _CFYDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED1R_25), 0, 25, State.elevon1R);      //DRAG DUE TO ELEVON 1R

        //SIDE FORCE DUE TO ELEVON 2L
        double _CFYDED2L;
        if (State.elevon2L < 0)
            _CFYDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED2L_16), 0, -16, State.elevon2L);      //DRAG DUE TO ELEVON 2L
        else
            _CFYDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED2L_25), 0, 25, State.elevon2L);      //DRAG DUE TO ELEVON 2L

        //SIDE FORCE DUE TO ELEVON 2R
        double _CFYDED2R;
        if (State.elevon2R < 0)
            _CFYDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED2R_16), 0, -16, State.elevon2R);      //DRAG DUE TO ELEVON 2R
        else
            _CFYDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CFYDED2R_25), 0, 25, State.elevon2R);      //DRAG DUE TO ELEVON 2R

        //SIDE FORCE DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CFYDSBL = getValue(alpha_deg, CFYDSBL) * State.airbrakes;
        //SIDE FORCE DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CFYDSBU = getValue(alpha_deg, CFYDSBU) * State.airbrakes;
        //SIDE FORCE DUE TO LE SLAT 1
        double _CFYDSD1L = getValue(alpha_deg, CFYDSD1L) * State.slats1;
        //SIDE FORCE DUE TO LE SLAT 2
        double _CFYDSD2L = getValue(alpha_deg, CFYDSD2L) * State.slats2;
        //SIDE FORCE INCREMENT DUE TO GEAR
        double _CFYGEAR = getValue(alpha_deg, CFYGEAR) * State.gear;

        _CYp = getValue(alpha_deg, CFYR);
        _CYr = getValue(alpha_deg, CFYP);
        _CY = _CFY1
            + _CFYDED1L + _CFYDED1R + _CFYDED2L + _CFYDED2R
            + _CFYDSBL + _CFYDSBU
            + _CFYDSD1L + _CFYDSD2L + _CFYGEAR ;
        CLog::cotrace("CFY,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _CY, _CFY1, _CFYDED1L, _CFYDED1R, _CFYDED2L, _CFYDED2R, _CFYDSBL, _CFYDSBU, _CFYDSD1L, _CFYDSD2L, _CFYGEAR, _CYp, _CYr);

// ROLLING MOMENT
        //BASIC ROLLING MOMENT
        double _CML1 = get2Value(alpha_deg, beta_deg, CML1);
        //ROLLING MOMENT DUE TO ELEVON 1L
        double _CMLDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CMLDED1L_0);
        if (State.elevon1L < 0)
            _CMLDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED1L_16), 0, -16, -State.elevon1L);
        else
            _CMLDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED1L_25), 0, 25, -State.elevon1L);

        //ROLLING MOMENT DUE TO ELEVON 1R
        double _CMLDED1R;
        if (State.elevon1R < 0)
            _CMLDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED1R_16), 0, -16, -State.elevon1R);      //DRAG DUE TO ELEVON 1R
        else
            _CMLDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED1R_25), 0, 25, -State.elevon1R);      //DRAG DUE TO ELEVON 1R

        //ROLLING MOMENT DUE TO ELEVON 2L
        double _CMLDED2L;
        if (State.elevon2L < 0)
            _CMLDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED2L_16), 0, -16, -State.elevon2L);      //DRAG DUE TO ELEVON 2L
        else
            _CMLDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED2L_25), 0, 25, -State.elevon2L);      //DRAG DUE TO ELEVON 2L

        //ROLLING MOMENT DUE TO ELEVON 2R
        double _CMLDED2R;
        if (State.elevon2R < 0)
            _CMLDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED2R_16), 0, -16, State.elevon2R);      //DRAG DUE TO ELEVON 2R
        else
            _CMLDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMLDED2R_25), 0, 25, State.elevon2R);      //DRAG DUE TO ELEVON 2R

        //ROLLING MOMENT DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CMLDSBL = getValue(alpha_deg, CMLDSBL) * State.airbrakes;
        //ROLLING MOMENT DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CMLDSBU = getValue(alpha_deg, CMLDSBU) * State.airbrakes;
        //ROLLING MOMENT DUE TO LE SLAT 1
        double _CMLDSD1L = getValue(alpha_deg, CMLDSD1L) * State.slats1;
        //ROLLING MOMENT DUE TO LE SLAT 2
        double _CMLDSD2L = getValue(alpha_deg, CMLDSD2L) * State.slats2;
        //ROLLING MOMENT INCREMENT DUE TO GEAR
        double _CMLGEAR = getValue(alpha_deg, CMLGEAR) * State.gear;


        _Clp = getValue(alpha_deg, CMLP);
        _Clr = getValue(alpha_deg, CMLR);
        _Cl = _CML1
            + _CMLDED1L + _CMLDED1R 
            + _CMLDED2L + _CMLDED2R
            + _CMLDSBL + _CMLDSBU
            + _CMLDSD1L + _CMLDSD2L + _CMLGEAR 
            ;
        CLog::cotrace("CML,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _Cl, _CML1, _CMLDED1L, _CMLDED1R, _CMLDED2L, _CMLDED2R, _CMLDSBL, _CMLDSBU, _CMLDSD1L, _CMLDSD2L, _CMLGEAR, _Clp, _Clr);
        
// YAW MOMENT
        //BASIC YAW MOMENT
        double _CMN1 = get2Value(alpha_deg, beta_deg, CMN1);
        //YAW MOMENT DUE TO ELEVON 1L
        double _CMNDED1L;
        //double z = get2Value(alpha_deg, beta_deg, CMNDED1L_0);
        if (State.elevon1L < 0)
            _CMNDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED1L_16), 0, -16, State.elevon1L);
        else
            _CMNDED1L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED1L_25), 0, 25, State.elevon1L);

        //YAW MOMENT DUE TO ELEVON 1R
        double _CMNDED1R;
        if (State.elevon1R < 0)
            _CMNDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED1R_16), 0, -16, State.elevon1R);      //DRAG DUE TO ELEVON 1R
        else
            _CMNDED1R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED1R_25), 0, 25, State.elevon1R);      //DRAG DUE TO ELEVON 1R

        //YAW MOMENT DUE TO ELEVON 2L
        double _CMNDED2L;
        if (State.elevon2L < 0)
            _CMNDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED2L_16), 0, -16, State.elevon2L);      //DRAG DUE TO ELEVON 2L
        else
            _CMNDED2L = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED2L_25), 0, 25, State.elevon2L);      //DRAG DUE TO ELEVON 2L

        //YAW MOMENT DUE TO ELEVON 2R
        double _CMNDED2R;
        if (State.elevon2R < 0)
            _CMNDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED2R_16), 0, -16, State.elevon2R);      //DRAG DUE TO ELEVON 2R
        else
            _CMNDED2R = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDED2R_25), 0, 25, State.elevon2R);      //DRAG DUE TO ELEVON 2R

        //YAWING MOMENT DUE TO RUDDER DEFLECTION
        double _CMNDRDr;
        if (State.rudder < 0)
            _CMNDRDr = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDRDr_N25), 0, -25, State.rudder);      //DRAG DUE TO ELEVON 2R
        else
            _CMNDRDr = linearInterpolation(0, get2Value(alpha_deg, beta_deg, CMNDRDr_25), 0, 25, State.rudder);      //DRAG DUE TO ELEVON 2R

        //YAW MOMENT DUE TO LOWER SPEEDBRAKE DEFLECTION
        double _CMNDSBL = getValue(alpha_deg, CMNDSBL) * State.airbrakes;
        //YAW MOMENT DUE TO UPPER SPEEDBRAKE DEFLECTION
        double _CMNDSBU = getValue(alpha_deg, CMNDSBU) * State.airbrakes;
        //YAW MOMENT DUE TO LE SLAT 1
        double _CMNDSD1L = getValue(alpha_deg, CMNDSD1L) * State.slats1;
        //YAW MOMENT DUE TO LE SLAT 2
        double _CMNDSD2L = getValue(alpha_deg, CMNDSD2L) * State.slats2;
        //YAW MOMENT INCREMENT DUE TO GEAR
        double _CMNGEAR = getValue(alpha_deg, CMNGEAR) * State.gear;

        _Cnp = getValue(alpha_deg, CMNR);
        _Cnr = getValue(alpha_deg, CMNP);
        _Cn = _CMN1
            + _CMNDED1L + _CMNDED1R + _CMNDED2L + _CMNDED2R
            + _CMNDRDr
            + _CMNDSBL + _CMNDSBU
            + _CMNDSD1L + _CMNDSD2L + _CMNGEAR ;
        CLog::cotrace("CMN,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", _Cn, _CMN1, _CMNDED1L, _CMNDED1R, _CMNDED2L, _CMNDED2R, _CMNDRDr, _CMNDSBL, _CMNDSBU, _CMNDSD1L, _CMNDSD2L, _CMNGEAR, _Cnp, _Cnr);
    }
    const double& CD() { return _CD; };
    const double& CY() { return _CY; };
    const double& CYp() { return _CYp; };
    const double& CYr() { return _CYr; };
    const double& CL() { return _CL; };
    const double& Cl() { return _Cl; };
    const double& Cm() { return _Cm; };
    const double& Cn() { return _Cn; };
    const double& Clp() { return _Clp; };
    const double& Clr() { return _Clr; };
    const double& Cmq() { return _Cmq; };
    const double& Cnp() { return _Cnp; };
    const double& Cnr() { return _Cnr; };
};