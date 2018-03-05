/************************************************************
 *
 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  EMAIL:
 *  fellowtraveler@opentransactions.org
 *
 *  WEBSITE:
 *  http://www.opentransactions.org/
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This Source Code Form is subject to the terms of the
 *   Mozilla Public License, v. 2.0. If a copy of the MPL
 *   was not distributed with this file, You can obtain one
 *   at http://mozilla.org/MPL/2.0/.
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will
 *   be useful, but WITHOUT ANY WARRANTY; without even the
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A
 *   PARTICULAR PURPOSE.  See the Mozilla Public License
 *   for more details.
 *
 ************************************************************/

#include "CmdActivitySummary.hpp"

#include <opentxs/api/Api.hpp>
#include <opentxs/api/Native.hpp>
#include <opentxs/api/UI.hpp>
#include <opentxs/client/OT_API.hpp>
#include <opentxs/core/Identifier.hpp>
#include <opentxs/core/Log.hpp>
#include <opentxs/core/Message.hpp>
#include <opentxs/core/String.hpp>
#include <opentxs/ui/ActivitySummary.hpp>
#include <opentxs/ui/ActivitySummaryItem.hpp>
#include <opentxs/OT.hpp>
#include <opentxs/Types.hpp>

#include <iostream>
#include <ctime>

namespace opentxs
{
CmdActivitySummary::CmdActivitySummary()
{
    command = "activitysummary";
    args[0] = "--mynym <nym>";
    category = catOtherUsers;
    help = "Display a summary of activity for a nym.";
}

std::int32_t CmdActivitySummary::runWithOptions()
{
    return run(getOption("mynym"));
}

std::int32_t CmdActivitySummary::run(std::string mynym)
{
    if (!checkNym("mynym", mynym)) {
        return -1;
    }

    const Identifier nymID{mynym};
    auto& activity = OT::App().UI().ActivitySummary(nymID);
    otOut << "Activity:\n";
    dashLine();
    auto& line = activity.First();

    if (false == line.Valid()) {

        return 1;
    }

    auto last = line.Last();
    otOut << "* " << line.DisplayName() << " (" << line.ThreadID() << "): "
          << time(line.Timestamp()) << "\n  " << line.Text() << "\n";

    while (false == last) {
        auto& line = activity.Next();
        last = line.Last();
        otOut << "* " << line.DisplayName() << " (" << line.ThreadID() << "): "
              << time(line.Timestamp()) << "\n  " << line.Text() << "\n";
    }

    otOut << std::endl;

    return 1;
}

std::string CmdActivitySummary::time(
    const std::chrono::system_clock::time_point in) const
{
    const auto converted = std::chrono::system_clock::to_time_t(in);

    return std::ctime(&converted);
}
} // namespace opentxs
