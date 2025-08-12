// default
#include <getopt.h>
#include <string>
using std::string;

// steam API
#include "steam/steam_api.h"

// random tools
#include "Util/AppID.h"

// main app
#include "Uploader.h"

// Tries to implement this:
// https://partner.steamgames.com/doc/features/workshop/implementation#uploading_a_workshop_item

// list of available parameters
static struct option long_options[] = {
    // default informations
    {"appID", required_argument, 0, 'a'},
    {"workshopID", required_argument, 0, 'w'},
    {"folder", required_argument, 0, 'f'},

    // parameters to upload
    {"description", required_argument, 0, 'd'},
    {"preview", required_argument, 0, 'p'},
    {"content", required_argument, 0, 'c'},
    {"title", required_argument, 0, 't'},
    {"visibility", required_argument, 0, 'v'},
    {"patchNote", required_argument, 0, 'P'},
    {"tags", required_argument, 0, 'T'},

    // verbose
    {"verbose", no_argument, 0, 'V'}, // does nothing for now
    {0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
    // parse command line arguments
    int opt;
    int option_index = 0;

    // init params
    AppId_t appID = 0;
    PublishedFileId_t workshopID = 0;
    string folder; // path to the folder

    string descriptionPath; // path to description
    string previewPath; // path to preview image
    string contentPath; // path to content folder
    string title; // title of the item
    ERemoteStoragePublishedFileVisibility visibility = static_cast<ERemoteStoragePublishedFileVisibility>(-1); // nil value to detect unset visibility
    string patchNotePath = ""; // path to patch note
    string tags = "$EMPTY"; // tags to add (not implemented yet)

    bool verbose = false;

    while ((opt = getopt_long(argc, argv, "a:w:f:d:p:c:t:v:P:V:T:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        // appID
        case 'a':
            appID = static_cast<AppId_t>(std::strtoull(optarg, nullptr, 10));
            break;

        // workshop ID
        case 'w':
            workshopID = static_cast<PublishedFileId_t>(std::strtoull(optarg, nullptr, 10));
            break;

        // mod folder
        case 'f':
            folder = string(optarg);
            break;


        // parameters
        case 'd': // description
            descriptionPath = string(optarg);
            break;
        case 'p': // preview
            previewPath = string(optarg);
            break;
        case 'c': // content folder
            contentPath = string(optarg);
            break;
        case 't': // title
            title = string(optarg);
            break;
        case 'v':
            visibility = static_cast<ERemoteStoragePublishedFileVisibility>(std::stoi(optarg));
            break;
        case 'P': // patch note
            patchNotePath = string(optarg);
            break;
        case 'T': // tags
            tags = string(optarg);
            break;

        // verbose
        case 'V':
            verbose = true;
            break;

        // wrong argument passed down
        default:
            std::cerr << "Wrong usage: " << argv[0] << " see doc.\n";
            return 1;
        }
    }

    if (workshopID == 0) {
        std::cerr << "Error: Provide a workshop ID.\n";
        return 1;
    }

    if (appID == 0) {
        appID = getAppID(workshopID);
        if (appID == 0) {
            std::cerr << "Error: Could not automatically retrieve appID from the workshop ID. Please provide it with --appID.\n";
            return 1;
        }
        std::cout << "Automatically retrieved appID from workshopID: " << appID << "\n";
    }

    if (descriptionPath.empty() && previewPath.empty() && contentPath.empty() && title.empty() && folder.empty() && visibility == static_cast<ERemoteStoragePublishedFileVisibility>(-1) && tags == "$EMPTY") {
        std::cerr << "Error: All parameters are empty. Nothing to upload.\n";
        return 1;
    }

    // upload item
    Uploader uploader(workshopID, appID);
    uploader.UpdateItem(descriptionPath, previewPath, contentPath, title, visibility, patchNotePath, tags);
    return 0;
}
