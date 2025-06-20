/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */


#include "basic.h"
#include "lexer.h"
#include "os.h"
#include "parser.h"

#include <windows.h>

DWORD WINAPI parser_proc(LPVOID param_ptr) {
    Parser *parser = (Parser *)param_ptr;
    Lexer *lexer = parser->lexer;

    for (;;) {
        while (lexer->next_index_to_read != lexer->next_index_to_write) {
            parser->parse();
        }
    }

    //return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        assert(0);
    } 

    // @Todo: Parse argv.

    char *filepath = argv[1];
    Lexer lexer{};
    Parser parser{};
    lexer.init(&parser);
    File_Handle file_handle = os_open_file(filepath);
    lexer.set_input_from_file(&file_handle);

    DWORD thread_id;
    HANDLE handle = CreateThread(NULL, 0, parser_proc, &parser, 0, &thread_id);

    lexer.tokenize();

    while (true) {}

    return 0;
}
