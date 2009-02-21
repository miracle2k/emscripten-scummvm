/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef SCI_VOCABULARY_H
#define SCI_VOCABULARY_H

#include "sci/include/versions.h"
#include "sci/include/sciresource.h"

namespace Sci {

struct ResourceManager;

/*#define VOCABULARY_DEBUG */
/*#define SCI_SIMPLE_SAID_CODE */ /* Whether the simplified Said() matching should be used */
/*#define SCI_SIMPLE_SAID_DEBUG */ /* uncomment to enable simple said debugging */


#define SCRIPT_UNKNOWN_FUNCTION_STRING "[Unknown]"
/* The string used to identify the "unknown" SCI0 function for each game */

#define PARSE_HEAP_SIZE 64
/* Number of bytes allocated on the heap to store bad words if parsing fails */


struct opcode {
	int type;
	int number;
	char* name;
};

#define VOCAB_RESOURCE_OPCODES 998
#define VOCAB_RESOURCE_KNAMES 999

#define VOCAB_RESOURCE_SCI0_MAIN_VOCAB 0
#define VOCAB_RESOURCE_SCI0_PARSE_TREE_BRANCHES 900
#define VOCAB_RESOURCE_SCI0_SUFFIX_VOCAB 901

#define VOCAB_RESOURCE_SCI1_MAIN_VOCAB 900
#define VOCAB_RESOURCE_SCI1_PARSE_TREE_BRANCHES 901
#define VOCAB_RESOURCE_SCI1_SUFFIX_VOCAB 902
#define VOCAB_RESOURCE_SCI1_CHAR_TRANSFORMS 913

#define VOCAB_CLASS_PREPOSITION 0x01
#define VOCAB_CLASS_ARTICLE 0x02
#define VOCAB_CLASS_ADJECTIVE 0x04
#define VOCAB_CLASS_PRONOUN 0x08
#define VOCAB_CLASS_NOUN 0x10
#define VOCAB_CLASS_INDICATIVE_VERB 0x20
#define VOCAB_CLASS_ADVERB 0x40
#define VOCAB_CLASS_IMPERATIVE_VERB 0x80
#define VOCAB_CLASS_NUMBER 0x001

extern const char *class_names[]; /* Vocabulary class names */

#define VOCAB_CLASS_ANYWORD 0xff
/* Anywords are ignored by the parser */

#define VOCAB_MAGIC_NUMBER_GROUP 0xffd /* 0xffe ? */
/* This word class is used for numbers */

#define VOCAB_TREE_NODES 500
/* Number of nodes for each parse_tree_node structure */

#define VOCAB_TREE_NODE_LAST_WORD_STORAGE 0x140
#define VOCAB_TREE_NODE_COMPARE_TYPE 0x146
#define VOCAB_TREE_NODE_COMPARE_GROUP 0x14d
#define VOCAB_TREE_NODE_FORCE_STORAGE 0x154

#define SAID_COMMA   0xf0
#define SAID_AMP     0xf1
#define SAID_SLASH   0xf2
#define SAID_PARENO  0xf3
#define SAID_PARENC  0xf4
#define SAID_BRACKO  0xf5
#define SAID_BRACKC  0xf6
#define SAID_HASH    0xf7
#define SAID_LT      0xf8
#define SAID_GT      0xf9
#define SAID_TERM    0xff

#define SAID_FIRST SAID_COMMA

/* There was no 'last matching word': */
#define SAID_FULL_MATCH 0xffff
#define SAID_NO_MATCH 0xfffe
#define SAID_PARTIAL_MATCH 0xfffd

#define SAID_LONG(x) ((x) << 8)

struct word_t {

	int w_class; /* Word class */
	int group; /* Word group */
	char word[1]; /* The actual word */

};


struct parse_rule_t {
	int id; /* non-terminal ID */
	int first_special; /* first terminal or non-terminal */
	int specials_nr; /* number of terminals and non-terminals */
	int length;
	int data[1]; /* actual data (size 1 to avoid compiler warnings) */
};


struct parse_rule_list_t {
	int terminal; /* Terminal character this rule matches against or 0 for a non-terminal rule */
	parse_rule_t *rule;
	parse_rule_list_t *next;
};


struct suffix_t {

	int class_mask; /* the word class this suffix applies to */
	int result_class; /* the word class a word is morphed to if it doesn't fail this check */

	int alt_suffix_length; /* String length of the suffix */
	int word_suffix_length; /* String length of the other suffix */

	char *alt_suffix; /* The alternative suffix */
	char *word_suffix; /* The suffix as used in the word vocabulary */

};


struct result_word_t {

	int w_class; /* Word class */
	int group; /* Word group */

};


struct synonym_t {
	int replaceant; /* The word group to replace */
	int replacement; /* The replacement word group for this one */
};


struct parse_tree_branch_t {

	int id;

	int data[10];

};

#define PARSE_TREE_NODE_LEAF 0
#define PARSE_TREE_NODE_BRANCH 1


struct parse_tree_node_t {

	short type;  /* leaf or branch */

	union {

		int value;  /* For leaves */
		short branches[2]; /* For branches */

	} content;

};




/*FIXME: These need freeing functions...*/

int *vocabulary_get_classes(ResourceManager *resmgr, int *count);

int vocabulary_get_class_count(ResourceManager *resmgr);

/**
 * Returns a null terminated array of selector names.
 */
char **vocabulary_get_snames(ResourceManager *resmgr, int *pcount, sci_version_t version);

/**
 * Frees the aforementioned array
 */
void vocabulary_free_snames(char **snames_list);

/* Look up a selector name in an array, return the index */
int vocabulary_lookup_sname(char **snames_list, char *sname);


/**
 * Returns a null terminated array of opcodes.
 */
opcode *vocabulary_get_opcodes(ResourceManager *resmgr);

void vocabulary_free_opcodes(opcode *opcodes);
/* Frees a previously allocated list of opcodes
** Parameters: (opcode *) opcodes: Opcodes to free
** Returns   : (void)
*/

/**
 * Returns a null terminated array of kernel function names.
 *
 * This function reads the kernel function name table from resource_map,
 * and returns a null terminated array of deep copies of them.
 * The returned array has the same format regardless of the format of the
 * name table of the resource (the format changed between version 0 and 1).
 */
char **vocabulary_get_knames(ResourceManager *resmgr, int* count);
void vocabulary_free_knames(char** names);



word_t **vocab_get_words(ResourceManager *resmgr, int *word_counter);
/* Gets all words from the main vocabulary
** Parameters: (ResourceManager *) resmr: The resource manager to read from
**             (int *) word_counter: The int which the number of words is stored in
** Returns   : (word_t **): A list of all words, dynamically allocated
*/


void
vocab_free_words(word_t **words, int words_nr);
/* Frees memory allocated by vocab_get_words
** Parameters: (word_t **) words: The words to free
**             (int) words_nr: Number of words in the structure
** Returns   : (void)
*/


suffix_t **vocab_get_suffices(ResourceManager *resmgr, int *suffices_nr);
/* Gets all suffixes from the suffix vocabulary
** Parameters: (ResourceManager*) resmgr: Resource manager the resources are
**                               read from
**             (int *) suffices_nr: The variable to store the number of suffices in
** Returns   : (suffix_t **): A list of suffixes
*/

void vocab_free_suffices(ResourceManager *resmgr, suffix_t **suffices, int suffices_nr);
/* Frees suffices_nr suffices
** Parameters: (ResourceManager *) resmgr: The resource manager to free from
**             (suffix_t **) suffices: The suffixes to free
**             (int) suffices_nr: Number of entrie sin suffices
** Returns   : (void)
*/

parse_tree_branch_t *vocab_get_branches(ResourceManager *resmgr, int *branches_nr);
/* Retrieves all grammar rules from the resource data
** Parameters: (ResourceManager*) resmgr: Resource manager the rules are
**                               read from
**             (int *) branches_nr: Pointer to the variable which the number of entries is to be
**                     stored in
** Returns   : (parse_tree_branch_t *): The rules, or NULL on error
*/

void vocab_free_branches(parse_tree_branch_t *parser_branches);
/* Frees all branches
** Parameters: (parse_tree_branch_t *) parser_branches: The branches to free
** Returns   : (null)
*/

result_word_t *vocab_lookup_word(char *word, int word_len,
                  word_t **words, int words_nr,
                  suffix_t **suffices, int suffices_nr);
/* Looks up a single word in the words and suffixes list
** Parameters: (char *) word: Pointer to the word to look up
**             (int) word_len: Length of the word to look up
**             (word_t **) words: List of words
**             (int) words_nr: Number of elements in 'words'
**             (suffix_t **) suffices: List of suffices
**             (int) suffices_nr: Number of entries in 'suffices'
** Returns   : (result_word_t *) A malloc'd result_word_t, or NULL if the word
** could not be found.
*/


result_word_t *vocab_tokenize_string(char *sentence, int *result_nr,
                      word_t **words, int words_nr,
                      suffix_t **suffices, int suffices_nr,
                      char **error);
/* Tokenizes a string and compiles it into word_ts.
** Parameters: (char *) sentence: The sentence to examine
**             (int *) result_nr: The variable to store the resulting number of words in
**             (word_t **) words: The words to scan for
**             (int) words_nr: Number of words to scan for
**             (suffix_t **) suffices: suffixes to scan for
**             (int) suffices_nr: Number of suffices to scan for
**             (char **) error: Points to a malloc'd copy of the offending text or to NULL on error
** Returns   : (word_t *): A list of word_ts containing the result, or NULL.
** On error, NULL is returned. If *error is NULL, the sentence did not contain any useful words;
** if not, *error points to a malloc'd copy of the offending word.
** The returned list may contain anywords.
*/


parse_rule_list_t *vocab_build_gnf(parse_tree_branch_t *branches, int branches_nr);
/* Constructs the Greibach Normal Form of the grammar supplied in 'branches'
** Parameters: (parse_tree_branch_t *) branches: The parser's branches
**             (int) branches_nr: Number of parser branches
** Returns   : (parse_rule_list_t *): Pointer to a list of singly linked
**                                    GNF rules describing the same language
**                                    that was described by 'branches'
** The original SCI rules are in almost-CNF (Chomsky Normal Form). Note that
** branch[0] is used only for a few magical incantations, as it is treated
** specially by the SCI parser.
*/


void vocab_free_rule_list(parse_rule_list_t *rule_list);
/* Frees a parser rule list as returned by vocab_build_gnf()
** Parameters: (parse_rule_list_t *) rule_list: The rule list to free
** Returns   : (void)
*/


int vocab_build_parse_tree(parse_tree_node_t *nodes, result_word_t *words, int words_nr,
                       parse_tree_branch_t *branch0, parse_rule_list_t *rules);
/* Builds a parse tree from a list of words
** Parameters: (parse_tree_node_t *) nodes: A node list to store the tree in (must have
**                                          at least VOCAB_TREE_NODES entries)
**             (result_word_t *) words: The words to build the tree from
**             (int) words_nr: The number of words
**             (parse_tree_branch_t *) branche0: The zeroeth original branch of the
**                                     original CNF parser grammar
**             (parse_rule_list *) rules: The GNF ruleset to parse with
** Returns   : 0 on success, 1 if the tree couldn't be built in VOCAB_TREE_NODES nodes
**             or if the sentence structure in 'words' is not part of the language
**             described by the grammar passed in 'rules'.
*/

void vocab_dump_parse_tree(const char *tree_name, parse_tree_node_t *nodes);
/* Prints a parse tree
** Parameters: (const char *) tree_name: Name of the tree to dump (free-form)
**             (parse_tree_node_t *) nodes: The nodes containing the parse tree
** Returns   : (void)
*/




int said(EngineState *s, byte *spec, int verbose);
/* Builds a parse tree from a spec and compares it to a parse tree
** Parameters: (EngineState *) s: The affected state
**             (byte *) spec: Pointer to the spec to build
**             (int) verbose: Whether to display the parse tree after building it
** Returns   : (int) 1 on a match, 0 otherwise
*/

const char *vocab_get_any_group_word(int group, word_t **words, int words_nr);
/* Gets any word from the specified group.
** Parameters: (int) group: Group number.
**             (word_t **) words: List of words
**             (int) words_nr: Count of words in the list.
** For debugging only.
*/


void vocab_decypher_said_block(EngineState *s, byte *pos);
/* Decyphers a said block and dumps its content via sciprintf.
** Parameters: (EngineState *) s: The state to use
**             (byte *) pos: Pointer to the data to dump
** For debugging only.
*/


void vocab_synonymize_tokens(result_word_t *words, int words_nr, synonym_t *synonyms, int synonyms_nr);
/* Synonymizes a token list
** Parameters: (result_wort_t *) words: The word list to synonymize
**             (int) words_nr: Number of word_ts in the list
**             (synonym_t *) synonyms: Synonym list
**             (int) synonyms_nr: Number of synonyms in the list
*/

int vocab_gnf_parse(parse_tree_node_t *nodes, result_word_t *words, int words_nr,
                parse_tree_branch_t *branch0, parse_rule_list_t *tlist, int verbose);

void vocab_gnf_dump(parse_tree_branch_t *branches, int branches_nr);

} // End of namespace Sci

#endif
