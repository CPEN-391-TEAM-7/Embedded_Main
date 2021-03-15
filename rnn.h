extern int Embed[39][4];
extern int Weight_Matrix[4][32];
extern int Recurrent_Matrix[32][32];
extern int Bias[32];
extern int Dense[32];
extern int Dense_Bias;
extern int char_list[256];

void initialize_rnn_params(void);
int test_rnn_input(char * test);


void load_rnn_weight_matrix_FPGA(void);
void load_rnn_recurrent_matrix_FPGA(void);
void load_rnn_bias_vector_FPGA(void);
void load_dense_weight_matrix_FPGA(void);
void load_dense_bias_FPGA(void);

void load_params_into_FPGA(void);

void rnn_apply_dense(void);


// RNN write offsets
#define rnn_start 0
#define  i_write 1
#define r0_write 2
#define r1_write 3
#define rb_write 4
#define  d_write 5
#define db_write 6
#define rnn_finish 7

// RNN read_offsets
#define  valid_read 0
#define   load_read 1
#define   dead_read 2
#define   beef_read 3
#define result_read 7
