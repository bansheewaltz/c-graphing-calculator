#ifndef INFIX_TO_POSTFIX_H_
#define INFIX_TO_POSTFIX_H_

#include "tkn_linkedl.h"
#include "tkn_queuearr.h"

TokenQueue* infix_to_postfix(const TokenNode* const list_head);

#endif  // INFIX_TO_POSTFIX_H_
