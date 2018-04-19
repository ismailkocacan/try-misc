﻿using System;
using System.Linq.Expressions;
class Program
{
    static void Main(string[] args)
    {
        ParameterExpression variableExp = Expression.Variable(typeof(int), "X");
        ConstantExpression consExp = Expression.Constant(5, typeof(int));
        BinaryExpression binaryExp = Expression.Assign(variableExp, consExp);

        ParameterExpression variableExp2 = Expression.Variable(typeof(int), "Y");
        ConstantExpression consExp2 = Expression.Constant(6, typeof(int));
        BinaryExpression binaryExp2 = Expression.Assign(variableExp2, consExp2);

        BinaryExpression exp3 = Expression.Add(variableExp, variableExp2);

        BlockExpression blockExp = Expression.Block(
            new ParameterExpression[] { variableExp, variableExp2 },
            binaryExp, binaryExp2, exp3
        );

        Console.WriteLine(blockExp.ToString());
        int result = Expression.Lambda<Func<int>>(blockExp).Compile().Invoke();
    }
}