#include "optimize.hh"
#include <assert.h>

/*** This file contains all code pertaining to AST optimisation. It currently
     implements a simple optimisation called "constant folding". Most of the
     methods in this file are empty, or just relay optimize calls downward
     in the AST. If a more powerful AST optimization scheme were to be
     implemented, only methods in this file should need to be changed. ***/


ast_optimizer *optimizer = new ast_optimizer();


/* The optimizer's interface method. Starts a recursive optimize call down
   the AST nodes, searching for binary operators with constant children. */
void ast_optimizer::do_optimize(ast_stmt_list *body)
{
    if (body != NULL)
        body->optimize();
}


/* Returns 1 if an AST expression is a subclass of ast_binaryoperation,
   ie, eligible for constant folding. You might want to change this
   method if you extend the folding beyond integers and reals.*/
int ast_optimizer::is_binop(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ADD:
    case AST_SUB:
    case AST_OR:
    case AST_AND:
    case AST_MULT:
    case AST_DIVIDE:
    case AST_IDIV:
    case AST_MOD:
        return 1;
    default:
        return 0;
    }
}



/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
void ast_node::optimize()
{
    fatal("Trying to optimize abstract class ast_node.");
}

void ast_statement::optimize()
{
    fatal("Trying to optimize abstract class ast_statement.");
}

void ast_expression::optimize()
{
    fatal("Trying to optimize abstract class ast_expression.");
}

void ast_lvalue::optimize()
{
    fatal("Trying to optimize abstract class ast_lvalue.");
}

void ast_binaryoperation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryoperation.");
}

void ast_binaryrelation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryrelation.");
}



/*** The optimize methods for the concrete AST classes. ***/

/* Optimize a statement list. This one you get for free.*/
void ast_stmt_list::optimize()
{
    if (preceding != NULL)
        preceding->optimize();
    if (last_stmt != NULL)
        last_stmt->optimize();
}


/* Optimize a list of expressions. */
void ast_expr_list::optimize()
{
    /* Your code here. */
    if (preceding != NULL)
        preceding->optimize();
    if (last_expr != NULL)
        last_expr->optimize();
}


/* Optimize an elsif list. */
void ast_elsif_list::optimize()
{
    /* Your code here. */
    if (preceding != NULL)
        preceding->optimize();
    if (last_elsif != NULL)
        last_elsif->optimize();
}


/* An identifier's value can change at run-time, so we can't perform
   constant folding optimization on it unless it is a constant.
   Thus we just do nothing here. It can be treated in the fold_constants()
   method, however. */
void ast_id::optimize()
{
}

void ast_indexed::optimize()
{
    /* Your code here. */
    index->optimize();
}


bool ast_optimizer::is_constant(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return sym_tab->get_symbol_tag(dynamic_cast<ast_id*>(node)->sym_p) == SYM_CONST;
    default:
        return false;
    }
}

bool ast_optimizer::is_foldable(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return is_constant(node);
    case AST_INTEGER:
    case AST_REAL:
        return true;

    default:
        return false;
    }
}

bool ast_optimizer::is_real(ast_expression *node)
{
   switch (node->tag)
    {
    case AST_ID:
        return sym_tab->get_symbol_type(dynamic_cast<ast_id*>(node)->sym_p) == real_type;
    case AST_REAL:
        return true;
    default:
        return false;
    }
}

float ast_optimizer::get_float(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return static_cast<float>(sym_tab->get_symbol(dynamic_cast<ast_id*>(node)->sym_p)->get_constant_symbol()->const_value.rval);
    case AST_INTEGER:
        return static_cast<float>(dynamic_cast<ast_integer*>(node)->value);
    case AST_REAL:
        return dynamic_cast<ast_real*>(node)->value;
    default:
        assert(false);
        return -1;
    }
}

int ast_optimizer::get_integer(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return static_cast<int>(sym_tab->get_symbol(dynamic_cast<ast_id*>(node)->sym_p)->get_constant_symbol()->const_value.ival);
    case AST_INTEGER:
        return dynamic_cast<ast_integer*>(node)->value;
    case AST_REAL:
        return static_cast<int>(dynamic_cast<ast_real*>(node)->value);
    default:
        assert(false);
        return -1;
    }
}

float ast_optimizer::do_operation_float(ast_binaryoperation *node)
{
    float left, right;
    left = get_float(node->left);
    right = get_float(node->right);
    switch (node->tag)
    {
    case AST_ADD:
        return left + right;
    case AST_SUB:
        return left - right;
    case AST_MULT:
        return left * right;
    case AST_DIVIDE:
        return left / right;
    default:
        assert(false);
        return 0;
    }
}

int ast_optimizer::do_operation_integer(ast_binaryoperation *node)
{
    int left, right;
    left = get_integer(node->left);
    right = get_integer(node->right);
    switch (node->tag)
    {
    case AST_ADD:
        return left + right;
    case AST_SUB:
        return left - right;
    case AST_OR:
        return left || right;
    case AST_AND:
        return left && right;
    case AST_MULT:
        return left * right;
    case AST_DIVIDE:
        assert(false);
        return left / right;
    case AST_IDIV:
        assert(false);
    case AST_MOD:
        return left % right;
    default:
        assert(false);
        return 0;
    }
}

/* This convenience method is used to apply constant folding to all
   binary operations. It returns either the resulting optimized node or the
   original node if no optimization could be performed. */
ast_expression *ast_optimizer::fold_constants(ast_expression *node)
{
    /* Your code here. */
    ast_expression *result = node;
    if (is_binop(node))
    {
        ast_binaryoperation *binop = dynamic_cast<ast_binaryoperation*>(node);
        if (is_foldable(binop->left) && is_foldable(binop->right))
        {
            if (is_real(binop->left) || is_real(binop->right))
            {
                float value = do_operation_float(binop);
                result = new ast_real(binop->left->pos, value);
            }
            else
            {
                int value = do_operation_integer(binop);
                result = new ast_integer(binop->left->pos, value);
            }
        }
    }
    return result;

}


void ast_add::optimize()
{
    /* Your code here. */
}

void ast_sub::optimize()
{
    /* Your code here. */
}

void ast_mult::optimize()
{
    /* Your code here. */
}

void ast_divide::optimize()
{
    /* Your code here. */
}

void ast_or::optimize()
{
    /* Your code here. */
}

void ast_and::optimize()
{
    /* Your code here. */
}

void ast_idiv::optimize()
{
    /* Your code here. */
}

void ast_mod::optimize()
{
    /* Your code here. */
}



/* We can apply constant folding to binary relations as well. */
void ast_equal::optimize()
{
    /* Your code here. */

}

void ast_notequal::optimize()
{
    /* Your code here. */

}

void ast_lessthan::optimize()
{
    /* Your code here. */

}

void ast_greaterthan::optimize()
{
    /* Your code here. */

}



/*** The various classes derived from ast_statement. ***/

void ast_procedurecall::optimize()
{
    /* Your code here. */
}


void ast_assign::optimize()
{
    /* Your code here. */

}


void ast_while::optimize()
{
    /* Your code here. */

}


void ast_if::optimize()
{
    /* Your code here. */

}


void ast_return::optimize()
{
    /* Your code here. */

}


void ast_functioncall::optimize()
{
    /* Your code here. */
}

void ast_uminus::optimize()
{
    /* Your code here. */

}

void ast_not::optimize()
{
    /* Your code here. */

}


void ast_elsif::optimize()
{
    /* Your code here. */

}



void ast_integer::optimize()
{
    /* Your code here. */
}

void ast_real::optimize()
{
    /* Your code here. */
}


void ast_cast::optimize()
{
    /* Your code here. */

}



void ast_procedurehead::optimize()
{
    fatal("Trying to call ast_procedurehead::optimize()");
}


void ast_functionhead::optimize()
{
    fatal("Trying to call ast_functionhead::optimize()");
}




