/*
   1 + 2 * 3
	'+'
   /   \
 '1'    *
      /   \
    '2'   '3'
-----------------------------------
1 + 2 * 3      => expression
x              => variable
x = 1 + 2 * 3  => assignment statement
  assignment node
   /            \
 variable       expression
  x                	'+'
                   /   \
                 '1'    *
                      /   \
                    '2'   '3'
-----------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int refCount = 0;
typedef void* Pointer;
typedef int* PInt;

const char* cASTNodeType[] = {
   "Assignment",
   "Variable",
   "Operator",
   "Value"
};

const char* cASTOperatorType[] = {
   "UnDefined",
   "+",
   "-",
   "/",
   "*"
};

typedef enum ASTNodeType{
    Assignment = 0,
    Variable = 1,
    Operator = 2,
    Value = 3
} ASTNodeType;

typedef enum ASTOperatorType{
    UnDefined = 0,
    Plus = 1,
    Minus = 2,
    Div = 3,
    Mul = 4
} ASTOperatorType;

typedef struct ASTNode{
   Pointer data;
   struct ASTNode* left;
   struct ASTNode* right;
   struct ASTNode* parent;
   ASTNodeType nodeType;
} ASTNode, *PASTNode;

typedef struct ASTOperatorNode{
    ASTOperatorType value;
} ASTOperatorNode, *PASTOperatorNode;

typedef struct ASTVariableNode{
   char variableName[20];
} ASTVariableNode, *PASTVariableNode;

Pointer new(size_t size){
   Pointer  p = malloc(size);
   refCount++;
   return p;
};

void delete(Pointer ptr){
   free(ptr);
   ptr = NULL;
   refCount--;
}

PASTNode createNode(ASTNodeType type,PASTNode parent){
    PASTNode node = new(sizeof(ASTNode));
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent;
    node->nodeType = type;
    return node;
};

float evaluate(PASTNode node){
    if (!node) return 0.0f;
    switch(node->nodeType){
        case Operator:{
            PASTOperatorNode operatorTypeData = (PASTOperatorNode)node->data;
            switch (operatorTypeData->value) {
                case Plus: return evaluate(node->left) + evaluate(node->right);
                case Minus: return evaluate(node->left) - evaluate(node->right);
                case Div: return evaluate(node->left) / evaluate(node->right);
                case Mul: return evaluate(node->left) * evaluate(node->right);
                default:{
                    return 0.0f;
                }
            }
        }
        case Value:{
            int value = (*(PInt)node->data);
            if (value != 0) return value;
        }
    }
}

float interpret(PASTNode node) {
    switch (node->nodeType){
        case Assignment:{
            PASTNode nodeVariable = node->left;
            PASTNode nodeOperator = node->right;
            return evaluate(nodeOperator);
        }
        case Operator:{
            return evaluate(node);
        }
        default:{
            return 0.0f;
        }
    }
}

void freeNode(PASTNode node){
    if (node){
        freeNode(node->left);
        freeNode(node->right);
        delete(node->data);
        delete(node);
    }
}

void walk(FILE* file,PASTNode node){
    if (node){
        const char* strType = cASTNodeType[node->nodeType];
        switch (node->nodeType){
            case Assignment:{
                fprintf(file,"%s %s \n",strType,"=");
                break;
            }
            case Value:{
                int value = (*(PInt)node->data);
                fprintf(file,"%s %d \n",strType,value);
                break;
            }
            case Operator:{
                PASTOperatorNode opData = (PASTOperatorNode)node->data;
                fprintf(file,"%s %s \n",strType, cASTOperatorType[opData->value]);
                break;
            }
            case Variable:{
                PASTVariableNode varNode = (PASTVariableNode)node->data;
                fprintf(file,"%s %s \n",strType, varNode->variableName);
                break;
            }
            default:{
                // ??
            }
        }
        walk(file,node->left);
        walk(file,node->right);
    }
}

void print(PASTNode node){
    FILE* file = fopen("ast.txt","w");
    if (file){
        walk(file,node);
        fclose(file);
    }
}

int main() {
    PASTNode nodeAssignment,
            nodeVariable,
            nodeOperatorPlus,
            nodeOperatorMul,
            nodeValue_1,
            nodeValue_2,
            nodeValue_3;

    nodeAssignment = createNode(Assignment,NULL); // ana düğüm

    nodeVariable = createNode(Variable,nodeAssignment);
    PASTVariableNode varNode = new(sizeof(ASTVariableNode));
    strcpy(varNode->variableName,"x");
    nodeVariable->data = varNode;

    nodeOperatorPlus = createNode(Operator,nodeAssignment);
    PASTOperatorNode opDataPlus = new(sizeof(ASTOperatorNode));
    opDataPlus->value = Plus;
    nodeOperatorPlus->data = opDataPlus;

    nodeOperatorMul = createNode(Operator,nodeOperatorPlus);
    PASTOperatorNode opDataMul = new(sizeof(ASTOperatorNode));
    opDataMul->value = Mul;
    nodeOperatorMul->data = opDataMul;

    nodeValue_1 = createNode(Value,nodeOperatorPlus);
    nodeValue_1->data = new(sizeof(int));
    (*(PInt)nodeValue_1->data) = 1;

    nodeValue_2 = createNode(Value,nodeOperatorMul);
    nodeValue_2->data = new(sizeof(int));
    (*(PInt)nodeValue_2->data) = 2;

    nodeValue_3 = createNode(Value,nodeOperatorMul);
    nodeValue_3->data = new(sizeof(int));
    (*(PInt)nodeValue_3->data) = 3;

    nodeOperatorPlus->left  = nodeValue_1;
    nodeOperatorPlus->right = nodeOperatorMul;
    nodeOperatorMul->left = nodeValue_2;
    nodeOperatorMul->right = nodeValue_3;

    nodeAssignment->left = nodeVariable;
    nodeAssignment->right = nodeOperatorPlus;

    float result = evaluate(nodeOperatorPlus);
    printf("result : %f",result);

    float result1 = interpret(nodeAssignment);
    printf("result1 : %f",result1);


    print(nodeAssignment);

    freeNode(nodeAssignment);

    PASTNode leakNode = createNode(Variable,NULL);
    if (refCount) fprintf(stderr, "%s (ref count:%d)", "Memory leak detected !\n",refCount);

    return 0;
}
