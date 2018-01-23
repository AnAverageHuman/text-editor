#include ".h"

const char *argp_program_bug_address = "anaveragehuman AT icurse DOT nl";
const char doc[] = "a text editor with the ability to edit over networks"
                   "\vClients must specify the port number to connect to.\n"
                   "Servers must specify the name of the file to edit.";
static char args_doc[] = "file";
struct arguments arguments = {0, "0.0.0.0", 0, 0};

static struct argp_option options[] = {
  {"server", 's', 0     , 0, "enable server functionality"}        ,
  {"ip"    , 'i', "IP"  , 0, "the IP to bind or connect to"}       ,
  {"port"  , 'p', "port", 0, "the port to bind or connect to"}     ,
  {"file"  , 'f', "file", 0, "the file to edit (server mode only)"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 's':
      arguments->servermode = 1;
      break;
    case 'i':
      arguments->ip = arg;
      break;
    case 'p':
      arguments->port = arg;
      break;
    case 'f':
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

  if (! (arguments.servermode || arguments.port) ) {
    die("Clients must supply at least the port number.", 1);
  }

  if (arguments.servermode && ! arguments.filename) {
    die("Server must specify the file to edit.", 1);
  } // XXX: clients ignore the filename; user should be aware of this


  if (arguments.servermode) {
    server();
  } else {
    client();
  }

  return 0;
}

