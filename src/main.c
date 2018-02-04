#include ".h"

const char *argp_program_bug_address = "anaveragehuman AT icurse DOT nl";
const char doc[] = "a text editor with the ability to edit over networks";
static char args_doc[] = "FILE";
struct arguments arguments = {"0.0.0.0", "0"};

static struct argp_option options[] = {
  {"ip"   , 'i', "IP"  , 0, "the IP to bind or connect to"}   ,
  {"port" , 'p', "port", 0, "the port to bind or connect to"} ,
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 'i':
      arguments->ip = arg;
      break;
    case 'p':
      arguments->port = arg;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num > 0) {
        argp_error(state, "expected at most one filename");
      }
      arguments->filename = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

char loop = 1;

int main( int argc, char *argv[] ){
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  client();
  return 0;
}

