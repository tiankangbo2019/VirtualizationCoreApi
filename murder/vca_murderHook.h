//
// Created by root on 2019/12/16.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_MURDERHOOK_H
#define VIRTUALIZATIONCOREAPI_VCA_MURDERHOOK_H

#include <iostream>
#include <utility>
#include <python2.7/Python.h>   //-I/usr/include/python2.7 -lpython2.7
#include <map>
#include <string>
#include <jsoncpp/json/json.h>  //apt-get install libjsoncpp-dev libjsoncpp1

using namespace std;


static struct murderCommand {
    const char *p2p = "p2p_command";  //cpu压力

} murder;

static struct p2pmethod {
    const char *startTracker = "start_tracker";
    const char *stopTracker = "stop_tracker";
    const char *produceSeed = "produce_seed";
    const char *startShare = "distribute_seed";
    const char *downloadFile = "download_file";
    const char *stopShare = "stop_share";

} p2pmthod;


class vca_murderHook {

public:
    vca_murderHook();

    ~vca_murderHook();

    PyObject *p2p_Module;

    /*start tracker*/
    bool startTracker();

    /*stop tracker*/
    bool stopTracker();

    /*produce seed*/
    bool produceSeed(const string &sourceFilePath,
                     const string &sourceSeedPath,
                     const string &localHost,
                     const string &targetHost,
                     const string &targetSeedPath);

    /*distribute seed, start share*/
    bool startShare(const string &sourceFilePath,
                    const string &sourceSeedPath,
                    const string &localHost);

    /*download file*/
    bool downloadFile(const string &downloadPath,
                      const string &localSeedPath,
                      const string &localHost);

    /*stop share*/
    bool stopShare();

};


#endif //VIRTUALIZATIONCOREAPI_VCA_MURDERHOOK_H
