#include <catch2/catch.hpp>
#include "../src/eval.h"
#include "../src/parser.h"

static rv_obj* eval_source(const char* source, rv_env* env) {
    rv_parser* parser = rv_parser_new(source);
    rv_obj* expression = rv_parse(parser);
    rv_parser_free(parser);
    return rv_eval(expression, env);
}

TEST_CASE("define variables", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("(define answer 42)", env)->integer_v == 42);
    REQUIRE(eval_source("answer", env)->integer_v == 42);
    rv_env_free(env);
}

TEST_CASE("arithmetic and comparison", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("(+ 1 2 3)", env)->integer_v == 6);
    REQUIRE(eval_source("(* 3 (- 10 4))", env)->integer_v == 18);
    REQUIRE(eval_source("(/ 5 2)", env)->real_v == Approx(2.5));
    REQUIRE(eval_source("(< 1 2 3)", env)->bool_v);
    REQUIRE_FALSE(eval_source("(> 1 2)", env)->bool_v);
    rv_env_free(env);
}

TEST_CASE("conditionals", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("(if #t 1 2)", env)->integer_v == 1);
    REQUIRE(eval_source("(if #f 1 2)", env)->integer_v == 2);
    rv_env_free(env);
}

TEST_CASE("begin and quote", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("(begin (define x 4) (+ x 3))", env)->integer_v == 7);
    rv_obj* quoted = eval_source("(quote (1 2))", env);
    REQUIRE(quoted->t == RV_TYPE_PAIR);
    REQUIRE(quoted->pair_v.car->integer_v == 1);
    rv_env_free(env);
}

TEST_CASE("anonymous functions", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("((fn (x) (* x x)) 6)", env)->integer_v == 36);
    rv_env_free(env);
}

TEST_CASE("define and call functions", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    eval_source("(define (square x) (* x x))", env);
    REQUIRE(eval_source("(square 9)", env)->integer_v == 81);
    rv_env_free(env);
}

TEST_CASE("recursive functions", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    eval_source("(define (factorial n) (if (< n 2) 1 (* n (factorial (- n 1)))))", env);
    REQUIRE(eval_source("(factorial 6)", env)->integer_v == 720);
    rv_env_free(env);
}

TEST_CASE("list functions", "[eval]") {
    rv_env* env = rv_env_new(NULL);
    rv_env_add_builtins(env);
    REQUIRE(eval_source("(car (list 7 8 9))", env)->integer_v == 7);
    REQUIRE(eval_source("(car (cdr (list 7 8 9)))", env)->integer_v == 8);
    rv_env_free(env);
}
