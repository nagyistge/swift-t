package exm.stc.ast.descriptor;

import exm.stc.ast.SwiftAST;
import exm.stc.ast.antlr.ExMParser;
import exm.stc.common.exceptions.TypeMismatchException;
import exm.stc.common.exceptions.UndefinedVariableException;
import exm.stc.common.exceptions.UserException;
import exm.stc.common.lang.Operators;
import exm.stc.common.lang.Types;
import exm.stc.common.lang.Types.ScalarUpdateableType;
import exm.stc.common.lang.Types.Type;
import exm.stc.common.lang.Var;
import exm.stc.frontend.Context;
import exm.stc.frontend.TypeChecker;

public class Update {
  public Update(Var target, SwiftAST expr, Operators.UpdateMode mode) {
    super();
    this.target = target;
    this.expr = expr;
    this.mode = mode;
  }

  private final Var target;
  private final SwiftAST expr;
  private final Operators.UpdateMode mode;
  
  
  public Var getTarget() {
    return target;
  }

  public SwiftAST getExpr() {
    return expr;
  }

  public Operators.UpdateMode getMode() {
    return mode;
  }

  /**
   * @param context
   * @return Concrete type of Rval expression
   * @throws UserException
   */
  public Type typecheck(Context context) throws UserException {
    Type expected = ScalarUpdateableType.asScalarFuture(
                            this.target.type());
    
    Type exprType = TypeChecker.findSingleExprType(context, expr);
    if (exprType.assignableTo(expected)) {
      return expected;
    } else {
      throw new TypeMismatchException(context, "in update of variable "
          + target.name() + " with type " + target.type().typeName()
          + " expected expression of type " + expected.typeName() 
          + " but got expression of type " + exprType);
    }
  }
  
  public static Update fromAST(Context context, SwiftAST tree) 
          throws UserException {
    assert(tree.getType() == ExMParser.UPDATE);
    assert(tree.getChildCount() == 3);
    SwiftAST cmd = tree.child(0);
    SwiftAST var = tree.child(1);
    SwiftAST expr = tree.child(2);
    
    assert(cmd.getType() == ExMParser.ID);
    assert(var.getType() == ExMParser.ID);
    
    
    Operators.UpdateMode mode = Operators.UpdateMode.fromString(context, cmd.getText());
    assert(mode != null);
    
    Var v = context.getDeclaredVariable(var.getText());
    if (v == null) {
      throw new UndefinedVariableException(context, "variable "
          + var.getType() + " is not defined");
    }
    
    if (!Types.isScalarUpdateable(v.type())) {
      throw new TypeMismatchException(context, "can only update" +
          " updateable variables: variable " + v.name() + " had " +
          " type " + v.type().typeName());
    }
    
    return new Update(v, expr, mode);
  }
                                
}
