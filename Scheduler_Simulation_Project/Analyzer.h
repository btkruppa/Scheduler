/***************************************************
    file:     Analyzer.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to analyze the
              scheduler's performance

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "ProcessTable.cpp"

class Analyzer
{
    public:
        float MeanTurnaroundTime(ProcessTable);
        float MeanResponseRatio(ProcessTable);
        float MeanResponseTime(ProcessTable);
        float MeanThroughPut(ProcessTable);
        float MeanWaitTime(ProcessTable);
        void  OutputAnalysis(ProcessTable);
};

