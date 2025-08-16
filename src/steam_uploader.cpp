// default
#include <getopt.h>
#include <string>
using std::string;

// steam API
#include "steam/steam_api.h"

// random tools
#include "Util/AppID.h"
#include "Util/Help.h"

// main app
#include "Uploader.h"

// Implements this:
// https://partner.steamgames.com/doc/features/workshop/implementation#uploading_a_workshop_item


// list of available parameters
static struct option long_options[] = {
    // default informations
    {"appID", required_argument, 0, 'a'},
    {"workshopID", required_argument, 0, 'w'},
    {"new", no_argument, 0, 'n'},

    // parameters to upload
    {"description", required_argument, 0, 'd'},
    {"preview", required_argument, 0, 'p'},
    {"content", required_argument, 0, 'c'},
    {"title", required_argument, 0, 't'},
    {"visibility", required_argument, 0, 'v'},
    {"tags", required_argument, 0, 'T'},

    {"addAppDependency", required_argument, 0, 'Q'},
    {"removeAppDependency", required_argument, 0, 'q'},
    {"addItemDependency", required_argument, 0, 'I'},
    {"removeItemDependency", required_argument, 0, 'i'},

    // optional
    {"patchNote", required_argument, 0, 'P'},
    {"language", required_argument, 0, 'L'},

    // verbose
    {"verbose", no_argument, 0, 'V'}, // does nothing for now

    // help
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

// Helper to generate short options string from `long_options`.
std::string get_short_options(const struct option* options) {
    std::ostringstream oss;
    for (int i = 0; options[i].name != nullptr; ++i) {
        if (options[i].val == 0) continue; // skip if no short option
        oss << static_cast<char>(options[i].val);
        if (options[i].has_arg == required_argument)
            oss << ":";
        else if (options[i].has_arg == optional_argument)
            oss << "::";
    }
    return oss.str();
}

// Helper to check if all upload parameters are empty/unset.
bool allUploadParamsEmpty(
    const std::string& descriptionPath,
    const std::string& previewPath,
    const std::string& contentPath,
    const std::string& title,
    ERemoteStoragePublishedFileVisibility visibility,
    const std::string& tags
) {
    return descriptionPath.empty() &&
           previewPath.empty() &&
           contentPath.empty() &&
           title.empty() &&
           visibility == static_cast<ERemoteStoragePublishedFileVisibility>(-1) &&
           tags == "$EMPTY";
}

int main(int argc, char *argv[])
{
    // parse command line arguments
    int opt;
    int option_index = 0;

    // init params
    AppId_t appID = 0;
    PublishedFileId_t workshopID = 0;
    bool isNew = false;

    string descriptionPath; // path to description
    string previewPath; // path to preview image
    string contentPath; // path to content folder
    string title; // title of the item
    ERemoteStoragePublishedFileVisibility visibility = static_cast<ERemoteStoragePublishedFileVisibility>(-1); // nil value to detect unset visibility
    string tags = "$EMPTY"; // tags to add (not implemented yet)

    string addAppDependency = "";
    string removeAppDependency = "";
    string addItemDependency = "";
    string removeItemDependency = "";

    string patchNotePath = ""; // path to patch note
    string language = "english"; // default language on Steam's side

    bool verbose = false;

    // retrieve short options
    std::string short_options = get_short_options(long_options);

    // parse command line arguments
    while ((opt = getopt_long(argc, argv, short_options.c_str(), long_options, &option_index)) != -1)
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

        // call for a new workshop item creation
        case 'n':
            isNew = true;
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
        case 'T': // tags
            tags = string(optarg);
            break;

        case 'Q': // add app dependency
            addAppDependency = string(optarg);
            break;
        case 'q': // remove app dependency
            removeAppDependency = string(optarg);
            break;
        case 'I': // add item dependency
            addItemDependency = string(optarg);
            break;
        case 'i': // remove item dependency
            removeItemDependency = string(optarg);
            break;

        // optional
        case 'P': // patch note
            patchNotePath = string(optarg);
            break;
        case 'L': // language
            language = string(optarg);
            break;

        // verbose
        case 'V':
            verbose = true;
            break;

        // help
        case 'h':
            printHelp();
            return 0;

        // wrong argument passed down
        default:
            std::cerr << "Wrong usage: " << argv[0] << " see doc.\n";
            return 1;
        }
    }

    // verify at least one of these parameters is set
    if (allUploadParamsEmpty(descriptionPath, previewPath, contentPath, title, visibility, tags)) {
        // acceptable behavior, creating an empty workshop item
        if (isNew) {
            Uploader uploader(workshopID, appID, isNew);
            return 0;
        }
        // improper behavior here however
        std::cerr << "Error: All necessary parameters are empty. Nothing to upload.\n";
        return 1;
    }

    // upload item
    Uploader uploader(workshopID, appID, isNew);
    return uploader.UpdateItem(
        // at least one of
        descriptionPath, previewPath, contentPath, title, visibility, tags,
        addAppDependency, removeAppDependency, addItemDependency, removeItemDependency,
        // optional
        patchNotePath, language
    );
}