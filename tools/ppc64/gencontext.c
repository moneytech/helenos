#include <stdio.h>
#include <stdint.h>

typedef uint32_t uint32_t;
typedef uint64_t uint64_t;
typedef uint64_t ipl_t;
typedef uint64_t uintptr_t;

#define __ppc64_TYPES_H__
#include "../../arch/ppc64/include/context.h"
#include "../../arch/ppc64/include/fpu_context.h"

#define FILENAME "../../arch/ppc64/include/context_offset.h"

int main(void)
{
	FILE *f;
	struct context *pctx = NULL;
	struct fpu_context *fpctx = NULL;
	

	f = fopen(FILENAME,"w");
	if (!f) {
		perror(FILENAME);
		return 1;
	}

	fprintf(f, "/* This file is automatically generated by %s. */\n", __FILE__);	

	fprintf(f,"/* struct context */\n");

	fprintf(f,"#define OFFSET_SP    0x%x\n", ((int) &pctx->sp) - (int) pctx);
	fprintf(f,"#define OFFSET_PC    0x%x\n", ((int) &pctx->pc) - (int) pctx);
	fprintf(f,"#define OFFSET_R2    0x%x\n", ((int) &pctx->r2) - (int) pctx);
	fprintf(f,"#define OFFSET_R13   0x%x\n", ((int) &pctx->r13) - (int) pctx);
	fprintf(f,"#define OFFSET_R14   0x%x\n", ((int) &pctx->r14) - (int) pctx);
	fprintf(f,"#define OFFSET_R15   0x%x\n", ((int) &pctx->r15) - (int) pctx);
	fprintf(f,"#define OFFSET_R16   0x%x\n", ((int) &pctx->r16) - (int) pctx);
	fprintf(f,"#define OFFSET_R17   0x%x\n", ((int) &pctx->r17) - (int) pctx);
	fprintf(f,"#define OFFSET_R18   0x%x\n", ((int) &pctx->r18) - (int) pctx);
	fprintf(f,"#define OFFSET_R19   0x%x\n", ((int) &pctx->r19) - (int) pctx);
	fprintf(f,"#define OFFSET_R20   0x%x\n", ((int) &pctx->r20) - (int) pctx);
	fprintf(f,"#define OFFSET_R21   0x%x\n", ((int) &pctx->r21) - (int) pctx);
	fprintf(f,"#define OFFSET_R22   0x%x\n", ((int) &pctx->r22) - (int) pctx);
	fprintf(f,"#define OFFSET_R23   0x%x\n", ((int) &pctx->r23) - (int) pctx);
	fprintf(f,"#define OFFSET_R24   0x%x\n", ((int) &pctx->r24) - (int) pctx);
	fprintf(f,"#define OFFSET_R25   0x%x\n", ((int) &pctx->r25) - (int) pctx);
	fprintf(f,"#define OFFSET_R26   0x%x\n", ((int) &pctx->r26) - (int) pctx);
	fprintf(f,"#define OFFSET_R27   0x%x\n", ((int) &pctx->r27) - (int) pctx);
	fprintf(f,"#define OFFSET_R28   0x%x\n", ((int) &pctx->r28) - (int) pctx);
	fprintf(f,"#define OFFSET_R29   0x%x\n", ((int) &pctx->r29) - (int) pctx);
	fprintf(f,"#define OFFSET_R30   0x%x\n", ((int) &pctx->r30) - (int) pctx);
	fprintf(f,"#define OFFSET_R31   0x%x\n", ((int) &pctx->r31) - (int) pctx);
	fprintf(f,"#define OFFSET_CR    0x%x\n", ((int) &pctx->cr) - (int) pctx);
	
	fprintf(f,"\n");

	fprintf(f,"#define OFFSET_FR14  0x%x\n", ((int) &fpctx->fr14) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR15  0x%x\n", ((int) &fpctx->fr15) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR16  0x%x\n", ((int) &fpctx->fr16) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR17  0x%x\n", ((int) &fpctx->fr17) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR18  0x%x\n", ((int) &fpctx->fr18) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR19  0x%x\n", ((int) &fpctx->fr19) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR20  0x%x\n", ((int) &fpctx->fr20) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR21  0x%x\n", ((int) &fpctx->fr21) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR22  0x%x\n", ((int) &fpctx->fr22) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR23  0x%x\n", ((int) &fpctx->fr23) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR24  0x%x\n", ((int) &fpctx->fr24) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR25  0x%x\n", ((int) &fpctx->fr25) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR26  0x%x\n", ((int) &fpctx->fr26) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR27  0x%x\n", ((int) &fpctx->fr27) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR28  0x%x\n", ((int) &fpctx->fr28) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR29  0x%x\n", ((int) &fpctx->fr29) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR30  0x%x\n", ((int) &fpctx->fr30) - (int) fpctx);
	fprintf(f,"#define OFFSET_FR31  0x%x\n", ((int) &fpctx->fr31) - (int) fpctx);
	fprintf(f,"#define OFFSET_FPSCR 0x%x\n", ((int) &fpctx->fpscr) - (int) fpctx);
	
	fclose(f);

	return 0;
}
