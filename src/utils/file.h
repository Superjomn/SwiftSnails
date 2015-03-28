//
//  file.h
//  SwiftSnails
//
//  Created by Chunwei on 3/28/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "common.h"
#include "string.h"

// thread-safe
void scan_file_by_line (
        FILE *file, 
        std::mutex& file_mut, 
        std::function<void(const std::string &line)> &&handler
    )
{
    LineFileReader line_reader;
    for(;;) {

        file_mut.lock();
        char* res = line_reader.getline(file);

        if(res != NULL) {
            std::string line = line_reader.get();
            file_mut.unlock();
            // gather keys
            handler(line);

        } else {
            file_mut.unlock();
            return;
        }
    }
}
    
