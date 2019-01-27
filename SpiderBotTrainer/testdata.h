#ifndef TESTDATA_H
#define TESTDATA_H

#include "sbmfooteditor.h"

#if(_WIN32)
    #define SBP_MODEL_DIR(s) QString("C:/workProjects/qtProjects/SpiderBotModel/models/").append(s)
#else
    #define SBP_MODEL_DIR(s) QString("/home/sergey/qtProjects/SpiderBotModel/models/").append(s)
#endif

sbmFootElement_t foot1Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,-60.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFootElement_t foot2Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,-90.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFootElement_t foot3Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,-120.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFootElement_t foot4Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,120.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFootElement_t foot5Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,90.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFootElement_t foot6Elements[] ={
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment1.stl")),
            QColor(10, 200, 10),
            1.7f, 96.f, -80.8f,
            0, -90, 90
        },
        37.2f,  //double length;
        0,0,60.f,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment2.stl")),
            QColor(10, 10, 200),
            92.7f, 37.4f, 1.6f,
            0.f, 180.f, 104
        },
        45.0,  //double length;
        37, 90, 0,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    },
    {
        {
            QUrl::fromLocalFile(SBP_MODEL_DIR("Segment3.stl")),
            QColor(200, 10, 200),
            -3.f, 92.95f, 1.6f,
            180,0,-172.5f
        },
        107.0,  //double length;
        0,0,-110,   //float rX, rY, rZ;
        10,  170, 90  //float angleMin, angleMax, angleDefault;
    }
};

sbmFoot_t foots[] = {
    { 3, foot1Elements , 22.6f, 68.f, 20.f},
    { 3, foot2Elements, 36.25f, 0.f, 20.f},
    { 3, foot3Elements, 22.6f, -68.f, 20.f},
    { 3, foot4Elements, -22.6f, -68.f, 20.f},
    { 3, foot5Elements, -36.25f, 0.f, 20.f},
    { 3, foot6Elements, -22.6f, 68.f, 20.f}
};

sbmBody_t body = {
    QColor(10, 200, 200),
    QUrl::fromLocalFile(SBP_MODEL_DIR("Body.stl"))
};

sbmSpiderBotSettings_t spiderBotSettings = {
    6, //footCount
    body,
    foots
};

#endif // TESTDATA_H
