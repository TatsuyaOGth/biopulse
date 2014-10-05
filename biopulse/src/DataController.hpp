#pragma once

#include "ofMain.h"

typedef struct _Data
{
    string timestamp;
    float voltage;
} Data;

typedef vector<Data *> dataset_type;

class DataSet
{
    const string mDataPath;
    dataset_type data;
    bool bDataLoaded;
    int mPointer;
    
public:

    DataSet(const string & path, bool header = true): mDataPath(path)
    {
        bDataLoaded = true;
        ofFile file(path);
        if (!file.exists()) {
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
        }
        mPointer = 0;
    }
    
    void updatePointer(){ mPointer++; }
    
    void getDataBetween(dataset_type * dst, int start, int end)
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
    vector<Data *> & getData() { return data; }
    Data * getCurrentData()
    {
        if (data.empty()) return NULL;
        if (mPointer < 0) return NULL;
        if (mPointer > data.size()) mPointer = mPointer - data.size() + 1; // loop
        return data[mPointer];
    }
    Data * getTargetData(int n)
    {
        int t = n + mPointer;
        if (data.empty()) return NULL;
        if (t < 0) return NULL;
        if (t > data.size()) t = t - data.size() + 1; // loop
        return data[t];
    }
    Data * getData(int i)
    {
        mPointer = i;
        return getCurrentData();
    }
    Data * getNextData() {
        mPointer++;
        return getCurrentData();
    }
    int getDataSize() { return data.size(); }
    
    
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
        int size = dir.listDir(dirPath);
        if (size == 0) {
            ofLogError() << "dataset dir is empty: " << dirPath;
            return false;
        }
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

namespace data
{
    static int bufferLength = 0;
    static float gain = 1.0;
}

#define DATA_CONTROLLER DataController::getInstance()
#define DATASET DataController::getInstance()->mDataSet
#define DATASET_IT vector<shared_ptr<DataSet> >::iterator
