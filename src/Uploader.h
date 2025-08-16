#pragma once

// steam API
#include "steam/steam_api.h"

// random tools
// #include "Util/ExecCmd.h"
// #include "Util/WarningHook.h"
#include "Util/ReadFile.h"
#include "Util/AppID.h"
#include "Util/HasLanguage.h"

// enums
#include "Enum/EResult.h"
#include "Enum/EItemUpdateStatus.h"

// other
#include <iostream>
#include <thread>
#include <string>
using std::string;
#include <filesystem>
using std::filesystem::exists;
using std::filesystem::is_directory;
using std::filesystem::is_regular_file;
using std::filesystem::file_size;
#include <vector>


// main uploader class
class Uploader
{
public:
    // INIT
    Uploader(PublishedFileId_t workshopID, AppId_t appID, bool isNew);

    // METHODS
    // main call
    int UpdateItem(
        // at least one of
        string descriptionPath, string previewPath, string contentPath, string title, ERemoteStoragePublishedFileVisibility visibility, string tags, 
        string addAppDependency, string removeAppDependency, string addItemDependency, string removeItemDependency,
        // optional
        string patchNotePath, string language
    );

    bool InitSteamAPI();
    bool ShutdownSteamAPI();
    bool CheckProgress(UGCUpdateHandle_t updateHandle, EItemUpdateStatus* previousUpdateStatus);

private:
    // MEMBERS
    PublishedFileId_t m_workshopID;
    AppId_t m_appID;
    bool m_isNew;

    // METHODS

    // Create a new workshop item which will define the `m_workshopID` of the Uploader instance.
    void CreateWorkshopItem();

    // Create the handle for updating various aspects of a workshop item.
    UGCUpdateHandle_t CreateUpdateHandle(PublishedFileId_t workshopID);

    // update item informations
    bool SetItemDescription(UGCUpdateHandle_t updateHandle, string descriptionPath);
    bool SetItemPreview(UGCUpdateHandle_t updateHandle, string previewPath);
    bool SetItemContent(UGCUpdateHandle_t updateHandle, string contentPath);
    bool SetItemTitle(UGCUpdateHandle_t updateHandle, string title);
    bool SetItemVisibility(UGCUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility visibility);
    bool SetTags(UGCUpdateHandle_t updateHandle, string tags);

    bool HandleAppDependencies(UGCUpdateHandle_t updateHandle, string addAppDependency, string removeAppDependency);
    bool HandleItemDependencies(UGCUpdateHandle_t updateHandle, string addItemDependency, string removeItemDependency);

    bool SetUploadLanguage(UGCUpdateHandle_t updateHandle, string language);

    // Submit the item update with the patch note `pchContent`.
    void SubmitItemUpdate(UGCUpdateHandle_t updateHandle, string pchContent);


    // CALLBACKS

    // Get the result of updating the workshop item.
    void OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* eResult, bool needUserAgreement);
    CCallResult<Uploader, SubmitItemUpdateResult_t> m_callResultSubmit;

    // Get the result of creating a new workshop item.
    void OnCreateWorkshopItemResult(CreateItemResult_t* eResult, bool needUserAgreement);
    CCallResult<Uploader, CreateItemResult_t> m_callResultCreate;
};