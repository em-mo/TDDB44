#include "semantic.hh"


semantic *type_checker = new semantic();


/* Used to check that all functions contain return statements.
   Static means that it is only visible inside this file.
   It is set to 0 in do_typecheck() (ie, every time we start type checking
   a new block) and set to 1 if we find an ast_return node. See below. */
static int has_return = 0;


/* Interface for type checking a block of code represented as an AST node. */
void semantic::do_typecheck(symbol *env, ast_stmt_list *body) {
    // Reset the variable, since we're checking a new block of code.
    has_return = 0;
    if(body)
	body->type_check();

    // This is the only case we need this variable for - a function lacking
    // a return statement. All other cases are already handled in
    // ast_return::type_check(); see below.
    if(env->tag == SYM_FUNC && has_return == 0) {
	// Note: We could do this by overloading the do_typecheck() method -
	// one for ast_procedurehead and one for ast_functionhead, but this
	// will do... Hopefully people won't write empty functions often,
	// since in that case we won't have position information available. 
	if(body != NULL)
	    type_error(body->pos) << "A function must return a value.\n";
	else
	    type_error() << "A function must return a value.\n";
    }
}

/* Compare formal vs. actual parameters recursively. */
int semantic::chk_param(ast_id *env,
			parameter_symbol *formals,
			ast_expr_list *actuals) {
    /* Your code here. */
    if (formals == NULL && actuals == NULL)
    {
        return 1;
    }
    else if (formals == NULL && actuals != NULL)
    {
        type_error(env->pos) << "Too many arguments.\n";
        return 0;
    }
    else if (formals != NULL && actuals == NULL)
    {
        type_error(env->pos) << "Too few arguments.\n";
        return 0;
    }
    else if (formals->type == actuals->last_expr->type)
    {
        return chk_param(env, formals->preceding, actuals->preceding);
    }
    else
    {
        type_error(env->pos) << "Actual arguments are not matching the formal arguments.\n";
        return 0;    
    }
    
}


/* Check formal vs. actual parameters at procedure/function calls. */
void semantic::check_function_parameters(ast_id *call_id,
				ast_expr_list *param_list) {
    /* Your code here. */
    function_symbol *func = sym_tab->get_symbol(call_id->sym_p)->get_function_symbol();
    parameter_symbol *formals = func->last_parameter;
    if (param_list != NULL)
    {
        param_list->type_check();
    }
    chk_param(call_id, formals, param_list);
}

/* Check formal vs. actual parameters at procedure/function calls. */
void semantic::check_procedure_parameters(ast_id *call_id,
                ast_expr_list *param_list) {
    /* Your code here. */
    procedure_symbol *proc = sym_tab->get_symbol(call_id->sym_p)->get_procedure_symbol();
    parameter_symbol *formals = proc->last_parameter;
    if (param_list != NULL)
    {
        param_list->type_check();
    }
    chk_param(call_id, formals, param_list);
}

/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
sym_index ast_node::type_check() {
    fatal("Trying to type check abstract class ast_node.");
    return void_type;
}

sym_index ast_statement::type_check() {
    fatal("Trying to type check abstract class ast_statement.");
    return void_type;
}

sym_index ast_expression::type_check() {
    fatal("Trying to type check abstract class ast_expression.");
    return void_type;
}

sym_index ast_lvalue::type_check() {
    fatal("Trying to type check abstract class ast_lvalue.");
    return void_type;
}

sym_index ast_binaryoperation::type_check() {
    fatal("Trying to type check abstract class ast_binaryoperation.");
    return void_type;
}

sym_index ast_binaryrelation::type_check() {
    fatal("Trying to type check abstract class ast_binaryrelation.");
    return void_type;
}



/* Type check a list of statements. */
sym_index ast_stmt_list::type_check() {
    if(preceding != NULL)
	preceding->type_check();
    if(last_stmt != NULL)
	last_stmt->type_check();
    return void_type;
}


/* Type check a list of expressions. */
sym_index ast_expr_list::type_check() {
    /* Your code here. */
    if(preceding != NULL)
        preceding->type_check();
    if(last_expr != NULL)
        last_expr->type_check();
    return void_type;
}



/* Type check an elsif list. */
sym_index ast_elsif_list::type_check() {
    /* Your code here. */
    if(preceding != NULL)
        preceding->type_check();
    if(last_elsif != NULL)
        last_elsif->type_check();
    return void_type;
}


/* "type check" an indentifier. We need to separate nametypes from other types
   here, since all nametypes are of type void, but should return an index to
   itself in the symbol table as far as typechecking is concerned. */
sym_index ast_id::type_check() {
    if(sym_tab->get_symbol(sym_p)->tag != SYM_NAMETYPE)
	return type;
    return sym_p;
}


sym_index ast_indexed::type_check() {
    /* Your code here. */
    if(index->type_check() != integer_type){
        type_error(index->pos) << "Array index must be of type integer\n";
    }
    return id->type_check();
}

/* This convenience function is used to type check all binary operations
   in which implicit casting of integer to real is done: plus, minus,
   multiplication. We synthesize type information as well. */   
sym_index semantic::check_binop1(ast_binaryoperation *node) {
    /* Your code here. You don't have to use this method, but it might be
       convenient. */
    sym_index left_type = node->left->type_check();
    sym_index right_type = node->right->type_check();
    if(left_type != right_type){
        if(left_type == integer_type)
        {
            node->left = new ast_cast(node->left->pos, node->left);
        }    
        else
        {
             node->right = new ast_cast(node->right->pos, node->right);
        }  
        node->type = real_type;
        return real_type; 
    }
    node->type = left_type;
    return left_type;
}

sym_index ast_add::type_check() {
    /* Your code here. */
    return type_checker->check_binop1(this);
    
}

sym_index ast_sub::type_check() {
    /* Your code here. */
    return type_checker->check_binop1(this);
    
}

sym_index ast_mult::type_check() {
    /* Your code here. */
    return type_checker->check_binop1(this);
    
}


sym_index ast_divide::type_check() {
    /* Your code here. */
    sym_index left_type = left->type_check();
    sym_index right_type = right->type_check();
    if(left_type == integer_type)
    {
        left = new ast_cast(left->pos, left);
    }    
    if(right_type == integer_type)
    {
         right = new ast_cast(right->pos, right);
    } 
    type = real_type;
    return real_type;
     
}



/* This convenience method is used to type check all binary operations
   which only accept integer operands: AND, OR, MOD, DIV.
   The second argument is the name of the operator, so we can generate a
   good error message.
   
   */
sym_index semantic::check_binop2(ast_binaryoperation *node, char *s) {
    /* Your code here. */
    if(node->left->type_check() != integer_type){
        type_error(node->left->pos) << "Left operand of " << s << " must be of type integer\n";
    }
    if(node->right->type_check() != integer_type){
        type_error(node->right->pos) << "Right operand of " << s << " must be of type integer\n";
    }
        node->type = integer_type;

    return integer_type;
    
}

sym_index ast_or::type_check() {
    /* Your code here. */
    return type_checker->check_binop2(this, "OR");
    
}

sym_index ast_and::type_check() {
    /* Your code here. */
    return type_checker->check_binop2(this, "AND");
        
}

sym_index ast_idiv::type_check() {
    /* Your code here. */
    return type_checker->check_binop2(this, "IDIV");
        
}

sym_index ast_mod::type_check() {
    /* Your code here. */
    return type_checker->check_binop2(this, "MOD");
        
}



/* Convienience method for all binary relations, since they're all typechecked
   the same way. They all return integer types, 1 = true, 0 = false. */
sym_index semantic::check_binrel(ast_binaryrelation *node) {
    /* Your code here. */
    sym_index left_type = node->left->type_check();
    sym_index right_type = node->right->type_check();
    if(left_type != right_type){
        if(left_type == integer_type)
        {
            node->left = new ast_cast(node->left->pos, node->left);
        }    
        else
        {
             node->right = new ast_cast(node->right->pos, node->right);
        }  
    }
    node->type = integer_type;
    return integer_type;
}

sym_index ast_equal::type_check() {
    /* Your code here. */
    return type_checker->check_binrel(this);
}

sym_index ast_notequal::type_check() {
    /* Your code here. */
    return type_checker->check_binrel(this);
}

sym_index ast_lessthan::type_check() {
    /* Your code here. */
    return type_checker->check_binrel(this);
}

sym_index ast_greaterthan::type_check() {
    /* Your code here. */
    return type_checker->check_binrel(this);
}



/*** The various classes derived from ast_statement. ***/

sym_index ast_procedurecall::type_check() {
    /* Your code here. */
    type_checker->check_procedure_parameters(id, parameter_list);
    return void_type;
}


sym_index ast_assign::type_check() {
    /* Your code here. */
    int lType, rType;
    lType = lhs->type_check();
    rType = rhs->type_check();

    if (lType == integer_type && rType == real_type)
        type_error(pos) << "Cannot assign real to integer.\n";
    else if (lType == void_type || rType == void_type)
        type_error(pos) << "Void cannot be in an assignment.\n";
    else if (lType == real_type && rType == integer_type)
        rhs = new ast_cast(rhs->pos, rhs);
    return lType;
    
}


sym_index ast_while::type_check() {
    if(condition->type_check() != integer_type)
	type_error(condition->pos) << "while predicate must be of integer "
				   << "type.\n";

    if(body != NULL)
	body->type_check();
    
    return void_type;
}
    

sym_index ast_if::type_check() {
    /* Your code here. */
    condition->type_check();
    if(body != NULL)
        body->type_check();
    if(elsif_list != NULL)
        elsif_list->type_check();
    if(else_body != NULL)
        else_body->type_check();
    return void_type;
    
}


sym_index ast_return::type_check() {
    symbol *tmp;
    sym_index value_type;

    // This static global (meaning it is global for all methods in this file,
    // but not visible outside this file) variable is reset to 0 every time
    // we start type checking a new block of code. If we find a return
    // statement, we set it to 1. It is checked in the do_typecheck() method
    // of semantic.cc.
    has_return = 1;
    
    // Get the current environment. We don't yet know if it's a procedure or
    // a function.
    tmp = sym_tab->get_symbol(sym_tab->current_environment());
    if(value == NULL) {
	// If the return value is NULL, 
	if(tmp->tag != SYM_PROC)
	    // ...and we're not inside a procedure, something is wrong.
	    type_error(pos) << "Must return a value from a function.\n";
	return void_type;
    }

    value_type = value->type_check();
    
    // The return value is not NULL,
    if(tmp->tag != SYM_FUNC) {
	// ...so if we're not inside a function, something is wrong too.
	type_error(pos) << "Procedures may not return a value.\n";
	return void_type;
    }
    
    // Now we know it's a function and can safely downcast.
    function_symbol *func = tmp->get_function_symbol();
    
    // Must make sure that the return type matches the function's
    // declared return type.
    if(func->type != value_type)
	type_error(value->pos) << "Bad return type from function.\n";
    
    return void_type;
}


sym_index ast_functioncall::type_check() {
    /* Your code here. */
    type_checker->check_function_parameters(id, parameter_list);
    if (type == void_type)
        type_error(pos) << "Functions must have a return value.\n";
    return type;
}

sym_index ast_uminus::type_check() {
    /* Your code here. */
    return expr->type_check();
}

sym_index ast_not::type_check() {
    /* Your code here. */
    if (expr->type_check() != integer_type)
        type_error(pos) << "Logical not operand must be of type integer.\n";
    return integer_type;
}


sym_index ast_elsif::type_check() {
    /* Your code here. */
    condition->type_check();
    body->type_check();
    return void_type;
}



sym_index ast_integer::type_check() {
    return integer_type;
}

sym_index ast_real::type_check() {
    return real_type;
}





	    
