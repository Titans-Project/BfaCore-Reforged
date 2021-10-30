#include "ScriptMgr.h"
#include "Chat.h"
#include "DB2Stores.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RBAC.h"
#include "WorldSession.h"

static bool HandlePlayMovieCommand(ChatHandler* handler, char const* args)
{
    if (!*args)
        return false;

    char const* movieIdStr = strtok((char*)args, " ");

    if (!movieIdStr)
        return false;

    uint32 movieId = atoi(movieIdStr);
    Player* target = handler->getSelectedPlayerOrSelf();

    if (!target)
    {
        handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
        handler->SetSentErrorMessage(true);
        return false;
    }

    if (!sMovieStore.HasRecord(movieId))
        return false;

    target->SendMovieStart(movieId);
    return true;
}

class movie_commandscript : public CommandScript
{
public:
    movie_commandscript() : CommandScript("movie_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> movieCommandTable =
        {
            { "play",    rbac::RBAC_PERM_COMMAND_MOVIE_PLAY, false, &HandlePlayMovieCommand,  "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "movie",          rbac::RBAC_PERM_COMMAND_MOVIE,              true, NULL,                             "", movieCommandTable }
        };
        return commandTable;
    }
};

void AddSC_movie_commandscript()
{
    new movie_commandscript();
}