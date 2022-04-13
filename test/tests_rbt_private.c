#include "cmocka_headers.h"

#include "cgs_rbt.h"
#include "cgs_rbt_private.h"
#include "cgs_compare.h"

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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
enum cgs_rbt_rules_rc {
	CGS_RBT_TEST_SUCCESS		= 0,
	CGS_RBT_TEST_FAIL_COLOR		= 1U,
	CGS_RBT_TEST_FAIL_ROOT		= 1U << 1,
	CGS_RBT_TEST_FAIL_RED_CHILDREN	= 1U << 2,
	CGS_RBT_TEST_FAIL_BLACK_HEIGHT	= 1U << 3,
};

int rbt_rules_test(const struct cgs_rbt_node* node)
{
	// 3. Leaves are black (or NULL in this implementation)
	if (!node)
		return CGS_RBT_TEST_SUCCESS;

	// 1. Every node is either red or black
	if (node->color != CGS_RBT_RED && node->color != CGS_RBT_BLACK) {
		return CGS_RBT_TEST_FAIL_COLOR;
	}

	// 2. The root is black
	if (node->parent == NULL && node->color != CGS_RBT_BLACK)
		return CGS_RBT_TEST_FAIL_ROOT;

	// 4. If a node is red, both its children are black
	if (!cgs_rbt_node_red_children_test(node))
		return CGS_RBT_TEST_FAIL_RED_CHILDREN;

	// 5. All simple paths from a node have the same black-height
	int left_height = cgs_rbt_node_black_height(node->left);
	int right_height = cgs_rbt_node_black_height(node->right);
	if (left_height != right_height || left_height < 0)
		return CGS_RBT_TEST_FAIL_BLACK_HEIGHT;

	return rbt_rules_test(node->left) | rbt_rules_test(node->right);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Hand-made trees
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

static int tree_teardown(void** state)
{
	struct cgs_rbt_node* node = *(struct cgs_rbt_node**)state;
	cgs_rbt_node_free(node);
	return 0;
}

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

/* fake_tree_bad_bh_small
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7B*
 */
static int fake_tree_bad_bh_small_setup(void** state)
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

/* fake_tree_bad_rc_small
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3R*             5B            __8B
 *          |                                        |
 *          1R                                       7R
 */
static int fake_tree_bad_rc_small_setup(void** state)
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

	assert_true(rbt_rules_test(root) == CGS_RBT_TEST_SUCCESS);
}

static void rbt_node_large_hand_test(void** state)
{
	struct cgs_rbt_node* root = *(struct cgs_rbt_node**)state;

	assert_int_equal(cgs_rbt_node_black_height(root), 4);
	assert_true(rbt_rules_test(root) == CGS_RBT_TEST_SUCCESS);
}

/*
int rbt_node_fail_test(void* data)
{
	(void)data;
	struct cgs_rbt_node* root_bh = fake_tree_bad_bh_small();
	assert(cgs_rbt_node_red_children_test(root_bh));
	assert(rbt_rules_test(root_bh) == CGS_RBT_TEST_FAIL_BLACK_HEIGHT);

	cgs_rbt_node_free(root_bh);

	struct cgs_rbt_node* root_rc = fake_tree_bad_rc_small();
	assert(rbt_rules_test(root_rc) == CGS_RBT_TEST_FAIL_RED_CHILDREN);

	cgs_rbt_node_free(root_rc);

	return TEST_SUCCESS;
}
*/

/*
int main(void)
{
	struct test tests[] = {
		{ "rbt_node_small_hand", rbt_node_small_hand_test, NULL },
		{ "rbt_node_large_hand", rbt_node_large_hand_test, NULL },
		{ "rbt_node_fail", rbt_node_fail_test, NULL },
	};

	return cgs_run_tests(tests);
}
*/

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(rbt_node_small_hand_test,
				fake_tree_good_small_setup, tree_teardown),
		cmocka_unit_test_setup_teardown(rbt_node_large_hand_test,
				fake_tree_good_large_setup, tree_teardown),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

