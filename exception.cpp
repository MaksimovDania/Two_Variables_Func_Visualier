#include "exception.h"

OperationData* tryInitOperationData (){
    OperationData* request;
    try {
        request = new OperationData;
    } catch (_exception) {
        return nullptr;
    }
    return request;
}

Result* tryInitResult (){
    Result* response;
    try {
        response = new Result;
    } catch (_exception) {
        return nullptr;
    }
    return response;
}

Union* tryInitunionForWork (){
    Union* unionForWork;
    try {
        unionForWork = new Union;
    }  catch (_exception) {
        return nullptr;
    }
    return unionForWork;
}
