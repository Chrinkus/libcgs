#include "cmocka_headers.h"

#include "cgs_rbt.h"
#include "cgs_rbt_private.h"
#include "cgs_compare.h"

#include <stdlib.h>     // malloc, free

/*
 * The procedure for this test group:
 *
 * 1. Construct good and bad trees by hand (not using tree-insert)
 * 2. Run diagnostics on the hand-made trees to ensure the diagnostics are
 *    correct.
 * 3. Construct a tree using 'insert'
 * 4. Run diagnostics on the insert-made trees to ensure that insertions,
 *    rotates and balances are correct.
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Rules Test
 *
 * This function will recursively walk a tree to ensure that each node meets
 * the 5 rules that define a correct red-black tree.
 *
 * 1. Every node is either red or black.
 * 2. The root is black.
 * 3. Leaves are black (or NULL in this implementation).
 * 4. If a node is red, both its children are black.
 * 5. All simple paths from a node have the same black-height.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
enum cgs_rbt_rules_rc {
	TEST_SUCCESS		= 0,
	TEST_FAIL_COLOR		= 1U,
	TEST_FAIL_ROOT		= 1U << 1,
	TEST_FAIL_RED_CHILDREN	= 1U << 2,
	TEST_FAIL_BLACK_HEIGHT	= 1U << 3,
};

unsigned rbt_rules_test(const struct cgs_rbt_node* node)
{
	unsigned res = TEST_SUCCESS;

	// 3. Leaves are black (or NULL in this implementation)
	if (!node)
		return res;

	// 1. Every node is either red or black
	if (node->color != CGS_RBT_RED && node->color != CGS_RBT_BLACK)
		res |= TEST_FAIL_COLOR;

	// 2. The root is black
	if (node->parent == NULL && node->color != CGS_RBT_BLACK)
		res |= TEST_FAIL_ROOT;

	// 4. If a node is red, both its children are black
	if (!cgs_rbt_node_red_children_test(node))
		res |= TEST_FAIL_RED_CHILDREN;

	// 5. All simple paths from a node have the same black-height
	int left_height = cgs_rbt_node_black_height(node->left);
	int right_height = cgs_rbt_node_black_height(node->right);
	if (left_height != right_height || left_height < 0)
		res |= TEST_FAIL_BLACK_HEIGHT;

	res |= rbt_rules_test(node->left);
	res |= rbt_rules_test(node->right);

	return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hand-made tree helpers
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum node_flag { ADD, IGNORE };
enum node_child { LEFT, RIGHT, NUMCHILDREN };
struct node_data {
	int flag;
	int val;
	int color;
};

void set_children(struct cgs_rbt_node* node, struct node_data arr[NUMCHILDREN])
{
	struct cgs_variant v = { 0 };
	if (arr[LEFT].flag == ADD) {
		cgs_variant_set_int(&v, arr[LEFT].val);
		node->left = cgs_rbt_node_new(&v);
		node->left->color = arr[LEFT].color;
		node->left->parent = node;
	}
	if (arr[RIGHT].flag == ADD) {
		cgs_variant_set_int(&v, arr[RIGHT].val);
		node->right = cgs_rbt_node_new(&v);
		node->right->color = arr[RIGHT].color;
		node->right->parent = node;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Good fake tree setups
 *
 * - One small tree
 * - One large tree
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/* fake_tree_good_small
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7R
 */
static int fake_tree_good_small_setup(void** state)
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	*state = root;
	return 0;
}

/* fake_tree_good_large
 *                      _____________26B_____________
 *                     |                             |
 *              ______17R______                _____41B______
 *             |               |              |              |
 *        ____14B___       ___21B___       __30R__          47B
 *       |          |     |         |     |       |
 *     _10R_      _16B   19B_      23B   28B    _38B_
 *    |     |    |           |                 |     |
 *   _7B   12B  15R         20R               35R   39R
 *  |
 *  3R
 *
 */
static int fake_tree_good_large_setup(void** state)
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 26);
	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root; // 26
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 17, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 41, .color = CGS_RBT_BLACK }
	});

	node = root->left; // 17
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 14, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 21, .color = CGS_RBT_BLACK }
	});

	node = root->left->left; // 14
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 10, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 16, .color = CGS_RBT_BLACK }
	});

	node = root->left->left->left; // 10
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 12, .color = CGS_RBT_BLACK }
	});

	node = root->left->left->left->left; // 7
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 3, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	node = root->left->left->right; // 16
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 15, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	node = root->left->right; // 21
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 19, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 23, .color = CGS_RBT_BLACK }
	});

	node = root->left->right->left; // 19
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 20, .color = CGS_RBT_RED },
	});

	node = root->right; // 41
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 30, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 47, .color = CGS_RBT_BLACK }
	});

	node = root->right->left; // 30
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 28, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 38, .color = CGS_RBT_BLACK }
	});

	node = root->right->left->right; // 38
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 35, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 39, .color = CGS_RBT_RED }
	});

	*state = root;
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Bad tree setups
 *
 * - Bad node color
 * - Bad root
 * - Bad black height
 * - Bad red children and black height (hard to separate)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/* fake_tree_bad_color_setup
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7(37)*
 *
 * The color is stored as an integer from an enum. Here I will just set it
 * to 37 since there is no other entries in the enumeration other than
 * red and black.
 */
static int fake_tree_bad_color_setup(void** state)
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = 37 },
		[RIGHT] = { .flag = IGNORE }
	});

	*state = root;
	return 0;
}

/* fake_tree_bad_root_setup
 *               _______________4R*_____________
 *              |                               |
 *       _______2B______                 _______6B______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7R
 */
static int fake_tree_bad_root_setup(void** state)
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_RED;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	*state = root;
	return 0;
}

/* fake_tree_bad_bh
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7B*
 */
static int fake_tree_bad_bh_setup(void** state)
	// violates black-height rule
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		// Changed left node to black
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = IGNORE }
	});

	*state = root;
	return 0;
}

/* fake_tree_bad_rc
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3R*             5B            __8B
 *          |                                        |
 *          1R                                       7R
 */
static int fake_tree_bad_rc_setup(void** state)
	// violates red child rule and black-height rule
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		// Changed left node to black
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	*state = root;
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Insert tree setup
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int rbt_insert_tree_setup(void** state)
{
        struct cgs_rbt* tree = malloc(sizeof(struct cgs_rbt));
	*tree = cgs_rbt_new(cgs_int_cmp, NULL);

	const int arr[] = {
		2, 17, 16, 1, 8, 14, 15, 5, 6, 10,
		3, 20, 11, 7, 9, 18, 12, 4, 13, 19,
	};

	struct cgs_variant v = { 0 };
	for (size_t i = 0; i < CGS_ARRAY_LENGTH(arr); ++i) {
		cgs_variant_set_int(&v, arr[i]);
		cgs_rbt_insert(tree, &v);
	}

	*state = tree;
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Tree teardown
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static int tree_node_teardown(void** state)
{
	struct cgs_rbt_node* node = *(struct cgs_rbt_node**)state;
	cgs_rbt_node_free(node, NULL);
	return 0;
}

static int tree_teardown(void** state)
{
	struct cgs_rbt* tree = *(struct cgs_rbt**)state;
	cgs_rbt_free(tree);
        free(tree);
	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Actual tests
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static void rbt_node_small_hand_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	assert_true(cgs_rbt_node_is_black(root));
	assert_true(cgs_rbt_node_is_red(root->left));
	assert_true(cgs_rbt_node_is_red(root->right));
	assert_true(cgs_rbt_node_is_black(root->left->left));
	assert_true(cgs_rbt_node_is_black(root->left->right));
	assert_true(cgs_rbt_node_is_black(root->right->left));
	assert_true(cgs_rbt_node_is_black(root->right->right));

	assert_false(cgs_rbt_node_is_black(root->left->left->right));
	assert_false(cgs_rbt_node_is_red(root->right->right->right));

	assert_int_equal(cgs_rbt_node_black_height(root), 3);

	unsigned res = rbt_rules_test(root);
	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, 0);
}

static void rbt_node_large_hand_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	assert_int_equal(cgs_rbt_node_black_height(root), 4);

	unsigned res = rbt_rules_test(root);
	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, 0);
}

static void rbt_node_fail_bh_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	unsigned res = rbt_rules_test(root);

	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, TEST_FAIL_BLACK_HEIGHT);
}

static void rbt_node_fail_rc_test(void** state)
	// hard to fail red children without violating black height
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	unsigned res = rbt_rules_test(root);

	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, TEST_FAIL_RED_CHILDREN);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, TEST_FAIL_BLACK_HEIGHT);
}

static void rbt_node_fail_root_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	unsigned res = rbt_rules_test(root);
	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, TEST_FAIL_ROOT);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, 0);
}

static void rbt_node_fail_color_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	unsigned res = rbt_rules_test(root);
	assert_int_equal(res & TEST_FAIL_COLOR, TEST_FAIL_COLOR);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, 0);
}

static void rbt_insert_test(void** state)
{
	struct cgs_rbt* tree = *(struct cgs_rbt**)state;

	unsigned res = rbt_rules_test(cgs_rbt_root(tree));
	assert_int_equal(res & TEST_FAIL_COLOR, 0);
	assert_int_equal(res & TEST_FAIL_ROOT, 0);
	assert_int_equal(res & TEST_FAIL_RED_CHILDREN, 0);
	assert_int_equal(res & TEST_FAIL_BLACK_HEIGHT, 0);

	// min-max test
	const int* min = cgs_rbt_min(tree);
	const int* max = cgs_rbt_max(tree);
	assert_int_equal(*min, 1);
	assert_int_equal(*max, 20);

	// search test
	struct cgs_variant v1 = { 0 };
        const struct cgs_variant* found = NULL;

	cgs_variant_set_int(&v1, 14);
	found = cgs_rbt_search(tree, &v1);
	assert_non_null(found);
	assert_int_equal(*(const int*)cgs_variant_get(found), 14);

	cgs_variant_set_int(&v1, 21);
	found = cgs_rbt_search(tree, &v1);
	assert_null(found);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		// Good hand-made trees
		cmocka_unit_test_setup_teardown(rbt_node_small_hand_test,
				fake_tree_good_small_setup, tree_node_teardown),
		cmocka_unit_test_setup_teardown(rbt_node_large_hand_test,
				fake_tree_good_large_setup, tree_node_teardown),

		// Bad hand-made trees
		cmocka_unit_test_setup_teardown(rbt_node_fail_bh_test,
				fake_tree_bad_bh_setup, tree_node_teardown),
		cmocka_unit_test_setup_teardown(rbt_node_fail_rc_test,
				fake_tree_bad_rc_setup, tree_node_teardown),
		cmocka_unit_test_setup_teardown(rbt_node_fail_root_test,
				fake_tree_bad_root_setup, tree_node_teardown),
		cmocka_unit_test_setup_teardown(rbt_node_fail_color_test,
				fake_tree_bad_color_setup, tree_node_teardown),

		// Insert tree
		cmocka_unit_test_setup_teardown(rbt_insert_test,
				rbt_insert_tree_setup, tree_teardown),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

