#include <llvm-c/Core.h>
#include <llvm-c/BitReader.h>
#include <stdio.h>

#include "defs.h"

int id = 0;

void readmod(byte *bc, int64_t sz) {
	LLVMMemoryBufferRef bcRef = 0;
	LLVMModuleRef llMod = 0;
    LLVMValueRef llVal = 0;
	size_t l = 0;

    if (debug) {
        printf("readmod: %p %ld [%lx]\n", (void *) bc, sz, sz);
        dump(bc, 0, sz, 4, (char *) "\t");
        printf("\t...\n");
        dump(bc, ((sz/16)-4)*16, sz + 16, 6, (char *) "\t");
    }

	bcRef = LLVMCreateMemoryBufferWithMemoryRange((const char *) bc, sz, "tmp", 0);
	if (bcRef == 0) {
		printf("could not create memory buffer\n");
		return;
	}

    if (debug) {
        printf("parsing file: %ld bytes\n", sz);
    }

	if (LLVMParseBitcode2(bcRef, &llMod)) {
		printf("could not parse bitcode\n");
		LLVMDisposeMemoryBuffer(bcRef);
		return;
	}

    const char *modName = LLVMGetSourceFileName(llMod, &l);

	printf("module: %s / %s / %s\n",
        modName, LLVMGetTarget(llMod), LLVMGetDataLayoutStr(llMod));

    char outName[256];
    id += 1;
    snprintf(outName, 256, "%05d-%s.ll", id, modName);

	for (int i = 0; outName[i] != 0; i++) {
		if (outName[i] == '/') {
			outName[i] = '-';
		}
	}

    char *err = 0;
    LLVMBool saved = LLVMPrintModuleToFile(llMod, outName, &err);
    if (saved != 0) {
        printf(" - failed to save %s: %s\n", outName, err);
        return;
    }

    // globals

    llVal = LLVMGetFirstGlobal(llMod);
    while (llVal != 0) {
        const char *fnName = LLVMGetValueName2(llVal, &l);
        LLVMTypeRef tyRef = LLVMTypeOf(llVal);
        LLVMBool isDecl = LLVMIsDeclaration(llVal);
        printf("\t%c: %s: %s\n", isDecl ? 'E' : 'G', fnName, LLVMPrintTypeToString(tyRef));

        // 
        // LLVMTypeKind typeKind = LLVMGetTypeKind(typeRef);
        llVal = LLVMGetNextGlobal(llVal);
    }

    // aliases

    llVal = LLVMGetFirstGlobalAlias(llMod);
    while (llVal != 0) {
        const char *fnName = LLVMGetValueName2(llVal, &l);
        LLVMTypeRef tyRef = LLVMTypeOf(llVal);
        printf("\tA: %s: %s\n", fnName, LLVMPrintTypeToString(tyRef));

        // 
        // LLVMTypeKind typeKind = LLVMGetTypeKind(typeRef);
        llVal = LLVMGetNextGlobalAlias(llVal);
    }

    // functions

    llVal = LLVMGetFirstFunction(llMod);
    while (llVal != 0) {
        const char *fnName = LLVMGetValueName2(llVal, &l);
        LLVMTypeRef tyRef = LLVMTypeOf(llVal);
        LLVMBool isDecl = LLVMIsDeclaration(llVal);
        printf("\t%c: %s: %s\n", isDecl ? 'D' : 'F', fnName, LLVMPrintTypeToString(tyRef));

        // if (isDecl == 0) {
        //    analyseFn(llVal);
        // }
        
        llVal = LLVMGetNextFunction(llVal);
    }

	LLVMDisposeMemoryBuffer(bcRef);
}
