#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 53
#define LARGE_STATE_COUNT 12
#define SYMBOL_COUNT 39
#define ALIAS_COUNT 0
#define TOKEN_COUNT 21
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 12
#define MAX_ALIAS_SEQUENCE_LENGTH 6
#define PRODUCTION_ID_COUNT 14

enum ts_symbol_identifiers {
  sym_comment = 1,
  anon_sym_LBRACE = 2,
  anon_sym_RBRACE = 3,
  anon_sym_import = 4,
  anon_sym_fn = 5,
  anon_sym_LPAREN = 6,
  anon_sym_RPAREN = 7,
  anon_sym_class = 8,
  anon_sym_print = 9,
  anon_sym_DOT = 10,
  anon_sym_COMMA = 11,
  anon_sym_EQ = 12,
  anon_sym_DASH = 13,
  anon_sym_BANG = 14,
  sym_identifier = 15,
  sym_number = 16,
  sym_string = 17,
  anon_sym_true = 18,
  anon_sym_false = 19,
  anon_sym_error = 20,
  sym_source_file = 21,
  sym_statement = 22,
  sym_block = 23,
  sym_import = 24,
  sym_function_declaration = 25,
  sym_class_declaration = 26,
  sym_print = 27,
  sym_expression = 28,
  sym_parenthesized_expression = 29,
  sym_access_list = 30,
  sym_call = 31,
  sym_param_list = 32,
  sym_argument_list = 33,
  sym_literal = 34,
  sym_assignment = 35,
  sym_bool = 36,
  aux_sym_source_file_repeat1 = 37,
  aux_sym_block_repeat1 = 38,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_comment] = "comment",
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
  [anon_sym_import] = "import",
  [anon_sym_fn] = "fn",
  [anon_sym_LPAREN] = "(",
  [anon_sym_RPAREN] = ")",
  [anon_sym_class] = "class",
  [anon_sym_print] = "print",
  [anon_sym_DOT] = ".",
  [anon_sym_COMMA] = ",",
  [anon_sym_EQ] = "=",
  [anon_sym_DASH] = "-",
  [anon_sym_BANG] = "!",
  [sym_identifier] = "identifier",
  [sym_number] = "number",
  [sym_string] = "string",
  [anon_sym_true] = "true",
  [anon_sym_false] = "false",
  [anon_sym_error] = "error",
  [sym_source_file] = "source_file",
  [sym_statement] = "statement",
  [sym_block] = "block",
  [sym_import] = "import",
  [sym_function_declaration] = "function_declaration",
  [sym_class_declaration] = "class_declaration",
  [sym_print] = "print",
  [sym_expression] = "expression",
  [sym_parenthesized_expression] = "parenthesized_expression",
  [sym_access_list] = "access_list",
  [sym_call] = "call",
  [sym_param_list] = "param_list",
  [sym_argument_list] = "argument_list",
  [sym_literal] = "literal",
  [sym_assignment] = "assignment",
  [sym_bool] = "bool",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_block_repeat1] = "block_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_comment] = sym_comment,
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
  [anon_sym_import] = anon_sym_import,
  [anon_sym_fn] = anon_sym_fn,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_class] = anon_sym_class,
  [anon_sym_print] = anon_sym_print,
  [anon_sym_DOT] = anon_sym_DOT,
  [anon_sym_COMMA] = anon_sym_COMMA,
  [anon_sym_EQ] = anon_sym_EQ,
  [anon_sym_DASH] = anon_sym_DASH,
  [anon_sym_BANG] = anon_sym_BANG,
  [sym_identifier] = sym_identifier,
  [sym_number] = sym_number,
  [sym_string] = sym_string,
  [anon_sym_true] = anon_sym_true,
  [anon_sym_false] = anon_sym_false,
  [anon_sym_error] = anon_sym_error,
  [sym_source_file] = sym_source_file,
  [sym_statement] = sym_statement,
  [sym_block] = sym_block,
  [sym_import] = sym_import,
  [sym_function_declaration] = sym_function_declaration,
  [sym_class_declaration] = sym_class_declaration,
  [sym_print] = sym_print,
  [sym_expression] = sym_expression,
  [sym_parenthesized_expression] = sym_parenthesized_expression,
  [sym_access_list] = sym_access_list,
  [sym_call] = sym_call,
  [sym_param_list] = sym_param_list,
  [sym_argument_list] = sym_argument_list,
  [sym_literal] = sym_literal,
  [sym_assignment] = sym_assignment,
  [sym_bool] = sym_bool,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym_block_repeat1] = aux_sym_block_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_LBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_import] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_fn] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_class] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_print] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DOT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COMMA] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BANG] = {
    .visible = true,
    .named = false,
  },
  [sym_identifier] = {
    .visible = true,
    .named = true,
  },
  [sym_number] = {
    .visible = true,
    .named = true,
  },
  [sym_string] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_true] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_false] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_error] = {
    .visible = true,
    .named = false,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym_statement] = {
    .visible = true,
    .named = true,
  },
  [sym_block] = {
    .visible = true,
    .named = true,
  },
  [sym_import] = {
    .visible = true,
    .named = true,
  },
  [sym_function_declaration] = {
    .visible = true,
    .named = true,
  },
  [sym_class_declaration] = {
    .visible = true,
    .named = true,
  },
  [sym_print] = {
    .visible = true,
    .named = true,
  },
  [sym_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_parenthesized_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_access_list] = {
    .visible = true,
    .named = true,
  },
  [sym_call] = {
    .visible = true,
    .named = true,
  },
  [sym_param_list] = {
    .visible = true,
    .named = true,
  },
  [sym_argument_list] = {
    .visible = true,
    .named = true,
  },
  [sym_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_assignment] = {
    .visible = true,
    .named = true,
  },
  [sym_bool] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_block_repeat1] = {
    .visible = false,
    .named = false,
  },
};

enum ts_field_identifiers {
  field_accessed = 1,
  field_accessor = 2,
  field_args = 3,
  field_body = 4,
  field_callable = 5,
  field_expression = 6,
  field_left = 7,
  field_msg = 8,
  field_name = 9,
  field_params = 10,
  field_path = 11,
  field_right = 12,
};

static const char * const ts_field_names[] = {
  [0] = NULL,
  [field_accessed] = "accessed",
  [field_accessor] = "accessor",
  [field_args] = "args",
  [field_body] = "body",
  [field_callable] = "callable",
  [field_expression] = "expression",
  [field_left] = "left",
  [field_msg] = "msg",
  [field_name] = "name",
  [field_params] = "params",
  [field_path] = "path",
  [field_right] = "right",
};

static const TSFieldMapSlice ts_field_map_slices[PRODUCTION_ID_COUNT] = {
  [1] = {.index = 0, .length = 1},
  [2] = {.index = 1, .length = 1},
  [3] = {.index = 2, .length = 1},
  [4] = {.index = 3, .length = 2},
  [5] = {.index = 5, .length = 1},
  [6] = {.index = 6, .length = 2},
  [7] = {.index = 8, .length = 1},
  [8] = {.index = 9, .length = 2},
  [9] = {.index = 11, .length = 1},
  [10] = {.index = 12, .length = 2},
  [11] = {.index = 14, .length = 2},
  [12] = {.index = 16, .length = 2},
  [13] = {.index = 18, .length = 3},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_path, 1},
  [1] =
    {field_msg, 1},
  [2] =
    {field_body, 1},
  [3] =
    {field_body, 2},
    {field_name, 1},
  [5] =
    {field_expression, 1},
  [6] =
    {field_left, 0},
    {field_right, 2},
  [8] =
    {field_callable, 0},
  [9] =
    {field_accessed, 0},
    {field_accessor, 2},
  [11] =
    {field_name, 1},
  [12] =
    {field_args, 2},
    {field_callable, 0},
  [14] =
    {field_body, 4},
    {field_name, 1},
  [16] =
    {field_body, 3},
    {field_name, 1},
  [18] =
    {field_body, 5},
    {field_name, 1},
    {field_params, 3},
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  [0] = 0,
  [1] = 1,
  [2] = 2,
  [3] = 3,
  [4] = 4,
  [5] = 5,
  [6] = 6,
  [7] = 7,
  [8] = 8,
  [9] = 9,
  [10] = 10,
  [11] = 11,
  [12] = 12,
  [13] = 13,
  [14] = 14,
  [15] = 15,
  [16] = 16,
  [17] = 17,
  [18] = 18,
  [19] = 19,
  [20] = 20,
  [21] = 21,
  [22] = 22,
  [23] = 23,
  [24] = 24,
  [25] = 25,
  [26] = 26,
  [27] = 27,
  [28] = 28,
  [29] = 29,
  [30] = 30,
  [31] = 31,
  [32] = 32,
  [33] = 33,
  [34] = 34,
  [35] = 35,
  [36] = 36,
  [37] = 37,
  [38] = 38,
  [39] = 39,
  [40] = 40,
  [41] = 41,
  [42] = 42,
  [43] = 43,
  [44] = 44,
  [45] = 45,
  [46] = 46,
  [47] = 47,
  [48] = 48,
  [49] = 49,
  [50] = 50,
  [51] = 51,
  [52] = 52,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(5);
      if (lookahead == '!') ADVANCE(19);
      if (lookahead == '"') ADVANCE(2);
      if (lookahead == '#') ADVANCE(6);
      if (lookahead == '(') ADVANCE(11);
      if (lookahead == ')') ADVANCE(12);
      if (lookahead == ',') ADVANCE(16);
      if (lookahead == '-') ADVANCE(18);
      if (lookahead == '.') ADVANCE(15);
      if (lookahead == '=') ADVANCE(17);
      if (lookahead == 'c') ADVANCE(26);
      if (lookahead == 'e') ADVANCE(38);
      if (lookahead == 'f') ADVANCE(21);
      if (lookahead == 'i') ADVANCE(28);
      if (lookahead == 'p') ADVANCE(33);
      if (lookahead == 't') ADVANCE(34);
      if (lookahead == '{') ADVANCE(7);
      if (lookahead == '}') ADVANCE(8);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(46);
      if (('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 1:
      if (lookahead == '"') ADVANCE(2);
      if (lookahead == '(') ADVANCE(11);
      if (lookahead == ')') ADVANCE(12);
      if (lookahead == 'e') ADVANCE(38);
      if (lookahead == 'f') ADVANCE(22);
      if (lookahead == 't') ADVANCE(34);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(1)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(46);
      if (('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 2:
      if (lookahead == '"') ADVANCE(47);
      if (lookahead == '\\') ADVANCE(4);
      if (lookahead != 0) ADVANCE(2);
      END_STATE();
    case 3:
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(3)
      if (('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 4:
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(2);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(6);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(anon_sym_import);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(anon_sym_fn);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_class);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(anon_sym_print);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(anon_sym_DOT);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(anon_sym_EQ);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(anon_sym_BANG);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'a') ADVANCE(41);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'a') ADVANCE(27);
      if (lookahead == 'n') ADVANCE(10);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'a') ADVANCE(27);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(48);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(49);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'i') ADVANCE(29);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'l') ADVANCE(20);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'l') ADVANCE(40);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'm') ADVANCE(32);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'n') ADVANCE(42);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(36);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'o') ADVANCE(37);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'p') ADVANCE(31);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(25);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(44);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(30);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(50);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(43);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(35);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 's') ADVANCE(13);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 's') ADVANCE(24);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 's') ADVANCE(39);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 't') ADVANCE(14);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 't') ADVANCE(9);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'u') ADVANCE(23);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(sym_identifier);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 46:
      ACCEPT_TOKEN(sym_number);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(46);
      END_STATE();
    case 47:
      ACCEPT_TOKEN(sym_string);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(anon_sym_true);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 49:
      ACCEPT_TOKEN(anon_sym_false);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    case 50:
      ACCEPT_TOKEN(anon_sym_error);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'z')) ADVANCE(45);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 0},
  [2] = {.lex_state = 0},
  [3] = {.lex_state = 0},
  [4] = {.lex_state = 0},
  [5] = {.lex_state = 0},
  [6] = {.lex_state = 0},
  [7] = {.lex_state = 0},
  [8] = {.lex_state = 0},
  [9] = {.lex_state = 0},
  [10] = {.lex_state = 0},
  [11] = {.lex_state = 0},
  [12] = {.lex_state = 0},
  [13] = {.lex_state = 0},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 0},
  [16] = {.lex_state = 0},
  [17] = {.lex_state = 0},
  [18] = {.lex_state = 0},
  [19] = {.lex_state = 0},
  [20] = {.lex_state = 0},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 0},
  [23] = {.lex_state = 0},
  [24] = {.lex_state = 1},
  [25] = {.lex_state = 0},
  [26] = {.lex_state = 0},
  [27] = {.lex_state = 0},
  [28] = {.lex_state = 1},
  [29] = {.lex_state = 0},
  [30] = {.lex_state = 0},
  [31] = {.lex_state = 0},
  [32] = {.lex_state = 0},
  [33] = {.lex_state = 0},
  [34] = {.lex_state = 1},
  [35] = {.lex_state = 1},
  [36] = {.lex_state = 1},
  [37] = {.lex_state = 1},
  [38] = {.lex_state = 0},
  [39] = {.lex_state = 0},
  [40] = {.lex_state = 0},
  [41] = {.lex_state = 3},
  [42] = {.lex_state = 0},
  [43] = {.lex_state = 0},
  [44] = {.lex_state = 0},
  [45] = {.lex_state = 0},
  [46] = {.lex_state = 3},
  [47] = {.lex_state = 3},
  [48] = {.lex_state = 0},
  [49] = {.lex_state = 3},
  [50] = {.lex_state = 0},
  [51] = {.lex_state = 0},
  [52] = {.lex_state = 3},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_comment] = ACTIONS(1),
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
    [anon_sym_import] = ACTIONS(1),
    [anon_sym_fn] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_class] = ACTIONS(1),
    [anon_sym_print] = ACTIONS(1),
    [anon_sym_DOT] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_EQ] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [anon_sym_BANG] = ACTIONS(1),
    [sym_identifier] = ACTIONS(1),
    [sym_number] = ACTIONS(1),
    [sym_string] = ACTIONS(1),
    [anon_sym_true] = ACTIONS(1),
    [anon_sym_false] = ACTIONS(1),
    [anon_sym_error] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(44),
    [sym_statement] = STATE(3),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_source_file_repeat1] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(3),
    [sym_comment] = ACTIONS(5),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [2] = {
    [sym_statement] = STATE(2),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_source_file_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(25),
    [sym_comment] = ACTIONS(27),
    [anon_sym_LBRACE] = ACTIONS(30),
    [anon_sym_import] = ACTIONS(33),
    [anon_sym_fn] = ACTIONS(36),
    [anon_sym_LPAREN] = ACTIONS(39),
    [anon_sym_class] = ACTIONS(42),
    [anon_sym_print] = ACTIONS(45),
    [sym_identifier] = ACTIONS(48),
    [sym_number] = ACTIONS(51),
    [sym_string] = ACTIONS(51),
    [anon_sym_true] = ACTIONS(54),
    [anon_sym_false] = ACTIONS(54),
    [anon_sym_error] = ACTIONS(54),
  },
  [3] = {
    [sym_statement] = STATE(2),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_source_file_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(57),
    [sym_comment] = ACTIONS(59),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [4] = {
    [sym_statement] = STATE(8),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_block_repeat1] = STATE(8),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_RBRACE] = ACTIONS(61),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [5] = {
    [sym_statement] = STATE(7),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_block_repeat1] = STATE(7),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_RBRACE] = ACTIONS(63),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [6] = {
    [sym_statement] = STATE(4),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_block_repeat1] = STATE(4),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_RBRACE] = ACTIONS(65),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [7] = {
    [sym_statement] = STATE(8),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_block_repeat1] = STATE(8),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_RBRACE] = ACTIONS(67),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [8] = {
    [sym_statement] = STATE(8),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [aux_sym_block_repeat1] = STATE(8),
    [anon_sym_LBRACE] = ACTIONS(69),
    [anon_sym_RBRACE] = ACTIONS(72),
    [anon_sym_import] = ACTIONS(74),
    [anon_sym_fn] = ACTIONS(77),
    [anon_sym_LPAREN] = ACTIONS(80),
    [anon_sym_class] = ACTIONS(83),
    [anon_sym_print] = ACTIONS(86),
    [sym_identifier] = ACTIONS(89),
    [sym_number] = ACTIONS(92),
    [sym_string] = ACTIONS(92),
    [anon_sym_true] = ACTIONS(95),
    [anon_sym_false] = ACTIONS(95),
    [anon_sym_error] = ACTIONS(95),
  },
  [9] = {
    [sym_statement] = STATE(27),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [10] = {
    [sym_statement] = STATE(29),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(98),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
  [11] = {
    [sym_statement] = STATE(25),
    [sym_block] = STATE(33),
    [sym_import] = STATE(33),
    [sym_function_declaration] = STATE(33),
    [sym_class_declaration] = STATE(33),
    [sym_print] = STATE(33),
    [sym_expression] = STATE(21),
    [sym_parenthesized_expression] = STATE(17),
    [sym_access_list] = STATE(17),
    [sym_call] = STATE(17),
    [sym_literal] = STATE(17),
    [sym_assignment] = STATE(33),
    [sym_bool] = STATE(14),
    [anon_sym_LBRACE] = ACTIONS(7),
    [anon_sym_import] = ACTIONS(9),
    [anon_sym_fn] = ACTIONS(11),
    [anon_sym_LPAREN] = ACTIONS(13),
    [anon_sym_class] = ACTIONS(15),
    [anon_sym_print] = ACTIONS(17),
    [sym_identifier] = ACTIONS(19),
    [sym_number] = ACTIONS(21),
    [sym_string] = ACTIONS(21),
    [anon_sym_true] = ACTIONS(23),
    [anon_sym_false] = ACTIONS(23),
    [anon_sym_error] = ACTIONS(23),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 2,
    ACTIONS(102), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(100), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [23] = 2,
    ACTIONS(106), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(104), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [46] = 2,
    ACTIONS(110), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(108), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [69] = 2,
    ACTIONS(114), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(112), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [92] = 2,
    ACTIONS(118), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(116), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [115] = 2,
    ACTIONS(122), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(120), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [138] = 2,
    ACTIONS(126), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    ACTIONS(124), 10,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_RPAREN,
      anon_sym_DOT,
      anon_sym_COMMA,
      sym_number,
      sym_string,
  [161] = 3,
    ACTIONS(128), 1,
      anon_sym_EQ,
    ACTIONS(120), 8,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      anon_sym_DOT,
      sym_number,
      sym_string,
    ACTIONS(122), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [185] = 4,
    ACTIONS(134), 1,
      anon_sym_LPAREN,
    ACTIONS(136), 1,
      anon_sym_DOT,
    ACTIONS(130), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      sym_number,
      sym_string,
    ACTIONS(132), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [210] = 4,
    ACTIONS(134), 1,
      anon_sym_LPAREN,
    ACTIONS(136), 1,
      anon_sym_DOT,
    ACTIONS(138), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      sym_number,
      sym_string,
    ACTIONS(140), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [235] = 4,
    ACTIONS(134), 1,
      anon_sym_LPAREN,
    ACTIONS(136), 1,
      anon_sym_DOT,
    ACTIONS(142), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      sym_number,
      sym_string,
    ACTIONS(144), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [260] = 2,
    ACTIONS(146), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(148), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [280] = 9,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(150), 1,
      anon_sym_RPAREN,
    ACTIONS(152), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(38), 1,
      sym_expression,
    STATE(43), 1,
      sym_argument_list,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [314] = 2,
    ACTIONS(154), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(156), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [334] = 2,
    ACTIONS(158), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(160), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [354] = 2,
    ACTIONS(162), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(164), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [374] = 9,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(166), 1,
      anon_sym_RPAREN,
    ACTIONS(168), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(40), 1,
      sym_expression,
    STATE(45), 1,
      sym_param_list,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [408] = 2,
    ACTIONS(170), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(172), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [428] = 2,
    ACTIONS(174), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(176), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [448] = 2,
    ACTIONS(178), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(180), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [468] = 2,
    ACTIONS(182), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(184), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [488] = 2,
    ACTIONS(138), 7,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_LBRACE,
      anon_sym_RBRACE,
      anon_sym_LPAREN,
      sym_number,
      sym_string,
    ACTIONS(140), 8,
      anon_sym_import,
      anon_sym_fn,
      anon_sym_class,
      anon_sym_print,
      sym_identifier,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
  [508] = 8,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(152), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(38), 1,
      sym_expression,
    STATE(50), 1,
      sym_argument_list,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [539] = 7,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(152), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(22), 1,
      sym_expression,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [567] = 7,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(152), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(40), 1,
      sym_expression,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [595] = 7,
    ACTIONS(13), 1,
      anon_sym_LPAREN,
    ACTIONS(152), 1,
      sym_identifier,
    STATE(14), 1,
      sym_bool,
    STATE(20), 1,
      sym_expression,
    ACTIONS(21), 2,
      sym_number,
      sym_string,
    ACTIONS(23), 3,
      anon_sym_true,
      anon_sym_false,
      anon_sym_error,
    STATE(17), 4,
      sym_parenthesized_expression,
      sym_access_list,
      sym_call,
      sym_literal,
  [623] = 4,
    ACTIONS(134), 1,
      anon_sym_LPAREN,
    ACTIONS(136), 1,
      anon_sym_DOT,
    ACTIONS(186), 1,
      anon_sym_RPAREN,
    ACTIONS(188), 1,
      anon_sym_COMMA,
  [636] = 3,
    ACTIONS(190), 1,
      anon_sym_RPAREN,
    ACTIONS(192), 1,
      anon_sym_COMMA,
    ACTIONS(120), 2,
      anon_sym_LPAREN,
      anon_sym_DOT,
  [647] = 3,
    ACTIONS(134), 1,
      anon_sym_LPAREN,
    ACTIONS(136), 1,
      anon_sym_DOT,
    ACTIONS(194), 1,
      anon_sym_RPAREN,
  [657] = 2,
    ACTIONS(196), 1,
      sym_identifier,
    STATE(51), 1,
      sym_param_list,
  [664] = 2,
    ACTIONS(190), 1,
      anon_sym_RPAREN,
    ACTIONS(192), 1,
      anon_sym_COMMA,
  [671] = 1,
    ACTIONS(198), 1,
      anon_sym_RPAREN,
  [675] = 1,
    ACTIONS(200), 1,
      ts_builtin_sym_end,
  [679] = 1,
    ACTIONS(202), 1,
      anon_sym_RPAREN,
  [683] = 1,
    ACTIONS(204), 1,
      sym_identifier,
  [687] = 1,
    ACTIONS(206), 1,
      sym_identifier,
  [691] = 1,
    ACTIONS(208), 1,
      anon_sym_LBRACE,
  [695] = 1,
    ACTIONS(210), 1,
      sym_identifier,
  [699] = 1,
    ACTIONS(212), 1,
      anon_sym_RPAREN,
  [703] = 1,
    ACTIONS(214), 1,
      anon_sym_RPAREN,
  [707] = 1,
    ACTIONS(216), 1,
      sym_identifier,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(12)] = 0,
  [SMALL_STATE(13)] = 23,
  [SMALL_STATE(14)] = 46,
  [SMALL_STATE(15)] = 69,
  [SMALL_STATE(16)] = 92,
  [SMALL_STATE(17)] = 115,
  [SMALL_STATE(18)] = 138,
  [SMALL_STATE(19)] = 161,
  [SMALL_STATE(20)] = 185,
  [SMALL_STATE(21)] = 210,
  [SMALL_STATE(22)] = 235,
  [SMALL_STATE(23)] = 260,
  [SMALL_STATE(24)] = 280,
  [SMALL_STATE(25)] = 314,
  [SMALL_STATE(26)] = 334,
  [SMALL_STATE(27)] = 354,
  [SMALL_STATE(28)] = 374,
  [SMALL_STATE(29)] = 408,
  [SMALL_STATE(30)] = 428,
  [SMALL_STATE(31)] = 448,
  [SMALL_STATE(32)] = 468,
  [SMALL_STATE(33)] = 488,
  [SMALL_STATE(34)] = 508,
  [SMALL_STATE(35)] = 539,
  [SMALL_STATE(36)] = 567,
  [SMALL_STATE(37)] = 595,
  [SMALL_STATE(38)] = 623,
  [SMALL_STATE(39)] = 636,
  [SMALL_STATE(40)] = 647,
  [SMALL_STATE(41)] = 657,
  [SMALL_STATE(42)] = 664,
  [SMALL_STATE(43)] = 671,
  [SMALL_STATE(44)] = 675,
  [SMALL_STATE(45)] = 679,
  [SMALL_STATE(46)] = 683,
  [SMALL_STATE(47)] = 687,
  [SMALL_STATE(48)] = 691,
  [SMALL_STATE(49)] = 695,
  [SMALL_STATE(50)] = 699,
  [SMALL_STATE(51)] = 703,
  [SMALL_STATE(52)] = 707,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(5),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(52),
  [11] = {.entry = {.count = 1, .reusable = false}}, SHIFT(49),
  [13] = {.entry = {.count = 1, .reusable = true}}, SHIFT(36),
  [15] = {.entry = {.count = 1, .reusable = false}}, SHIFT(47),
  [17] = {.entry = {.count = 1, .reusable = false}}, SHIFT(35),
  [19] = {.entry = {.count = 1, .reusable = false}}, SHIFT(19),
  [21] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [23] = {.entry = {.count = 1, .reusable = false}}, SHIFT(15),
  [25] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2),
  [27] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(2),
  [30] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(5),
  [33] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(52),
  [36] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(49),
  [39] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(36),
  [42] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(47),
  [45] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(35),
  [48] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(19),
  [51] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(14),
  [54] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(15),
  [57] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1),
  [59] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [61] = {.entry = {.count = 1, .reusable = true}}, SHIFT(26),
  [63] = {.entry = {.count = 1, .reusable = true}}, SHIFT(32),
  [65] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [67] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [69] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(5),
  [72] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2),
  [74] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(52),
  [77] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(49),
  [80] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(36),
  [83] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(47),
  [86] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(35),
  [89] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(19),
  [92] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(14),
  [95] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_block_repeat1, 2), SHIFT_REPEAT(15),
  [98] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [100] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call, 4, .production_id = 10),
  [102] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_call, 4, .production_id = 10),
  [104] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_access_list, 3, .production_id = 8),
  [106] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_access_list, 3, .production_id = 8),
  [108] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_literal, 1),
  [110] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_literal, 1),
  [112] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_bool, 1),
  [114] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_bool, 1),
  [116] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call, 3, .production_id = 7),
  [118] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_call, 3, .production_id = 7),
  [120] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 1),
  [122] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_expression, 1),
  [124] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_parenthesized_expression, 3, .production_id = 5),
  [126] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_parenthesized_expression, 3, .production_id = 5),
  [128] = {.entry = {.count = 1, .reusable = true}}, SHIFT(37),
  [130] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_assignment, 3, .production_id = 6),
  [132] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_assignment, 3, .production_id = 6),
  [134] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [136] = {.entry = {.count = 1, .reusable = true}}, SHIFT(46),
  [138] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_statement, 1),
  [140] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_statement, 1),
  [142] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_print, 2, .production_id = 2),
  [144] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_print, 2, .production_id = 2),
  [146] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_block, 3, .production_id = 3),
  [148] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_block, 3, .production_id = 3),
  [150] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [152] = {.entry = {.count = 1, .reusable = false}}, SHIFT(17),
  [154] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_declaration, 6, .production_id = 13),
  [156] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_function_declaration, 6, .production_id = 13),
  [158] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_class_declaration, 5, .production_id = 12),
  [160] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_class_declaration, 5, .production_id = 12),
  [162] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_declaration, 5, .production_id = 11),
  [164] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_function_declaration, 5, .production_id = 11),
  [166] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [168] = {.entry = {.count = 1, .reusable = false}}, SHIFT(39),
  [170] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_declaration, 3, .production_id = 4),
  [172] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_function_declaration, 3, .production_id = 4),
  [174] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_class_declaration, 4, .production_id = 9),
  [176] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_class_declaration, 4, .production_id = 9),
  [178] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_import, 2, .production_id = 1),
  [180] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_import, 2, .production_id = 1),
  [182] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_block, 2),
  [184] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_block, 2),
  [186] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 1),
  [188] = {.entry = {.count = 1, .reusable = true}}, SHIFT(34),
  [190] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_param_list, 1),
  [192] = {.entry = {.count = 1, .reusable = true}}, SHIFT(41),
  [194] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [196] = {.entry = {.count = 1, .reusable = true}}, SHIFT(42),
  [198] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [200] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [202] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [204] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [206] = {.entry = {.count = 1, .reusable = true}}, SHIFT(48),
  [208] = {.entry = {.count = 1, .reusable = true}}, SHIFT(6),
  [210] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [212] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 3),
  [214] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_param_list, 3),
  [216] = {.entry = {.count = 1, .reusable = true}}, SHIFT(31),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_flamingo(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .field_names = ts_field_names,
    .field_map_slices = ts_field_map_slices,
    .field_map_entries = ts_field_map_entries,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .primary_state_ids = ts_primary_state_ids,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
