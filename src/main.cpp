#include "application.h"

#include <gtk/gtk.h>

using namespace usnake;

int main(int argc, char *argv[]) {
    Application application = Application();
    int status = application.start(argc, argv);

    return status;
}
