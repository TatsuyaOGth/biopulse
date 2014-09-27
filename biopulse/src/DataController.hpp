#pragma once

#include "ofMain.h"

typedef struct _Data
{
    string timestamp;
    float voltage;
} Data;

class DataSet
{
    const string mDataPath;
    vector<Data *> data;
    bool bDataLoaded;
    
public:

    DataSet(const string & path, bool header = true): mDataPath(path)
    {
        bDataLoaded = true;
        ofFile file(path);
        if (file.exists()) {
            bDataLoaded = false;
            return;
        }
        ofBuffer buffer(file);
        unsigned long tmpCount = 0;
        while (!buffer.isLastLine()) {
            string line = buffer.getNextLine();
            if (header) {
                header = false;
                continue;
            }
            string replaceTarget = "\"";
            string::size_type pos = line.find(replaceTarget);
            while (pos != string::npos) {
                line.replace(pos, replaceTarget.size(), "");
                pos = line.find(replaceTarget, pos);
            }
            vector<string> col = ofSplitString(line, ",");
            if (col.size() < 2) {
                ofLogWarning() << "column is low";
                continue;
            }
            data.push_back(new Data);
            data.back()->timestamp = col[0];
            data.back()->voltage = ofToFloat(col[1]);
        }
        if (data.empty()) {
            ofLogWarning() << "dataset is empty: " << path;
            bDataLoaded = false;
        } else {
            ofLogNotice() << "load dataset: " << path;
            bDataLoaded = false;
        }
    }
    
    void getDataBetween(vector<Data *> * dst, int start, int end)
    {
        if (start < 0 || start >= data.size()) return;
        if (end   < 0 || end   >= data.size()) return;
        if (start >= end) return;
        
        int size = end - start;
        dst->clear();
        for (int i = start; i < end; i++) {
            dst->push_back(data[i]);
        }
    }
    
    bool isDataLoaded() { return bDataLoaded; }
    
};

class DataController
{
    // singleton
    DataController(){};
    
    typedef shared_ptr<DataSet> dataset;
    
public:
    vector<dataset> mDataSet;
    
public:
    static DataController * getInstance()
    {
        static DataController * instance = new DataController();
        return instance;
    }
    
    bool loadDataSet(const string & dirPath)
    {
        ofDirectory dir;
        dir.listDir(dirPath);
        for (int i = 0; i < dir.size(); i++) {
            if (!addDataSet(dir.getPath(i))) {
                return false;
            }
        }
        return true;
    }
    
    bool addDataSet(const string & path)
    {
        dataset d = dataset(new DataSet(path));
        if (d->isDataLoaded()) {
            mDataSet.push_back(d);
        } else return false;
        return true;
    }
    
};

#define DATA_CONTROLLER DataController::getInstance()
#define DATASET DataController::getInstance().mDataSet
