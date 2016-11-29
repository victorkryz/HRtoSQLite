/**
 * HRtoSQLite_tests
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef HRTOSQLITE_ROOT_SELECTIONTS_H
#define HRTOSQLITE_ROOT_SELECTIONTS_H


#include "Common.h"
#include "Ora/Connection.h"
#include "Ora/Utils.h"
#include "Ora/Selector.h"
#include <Ora/Reader.h>
#include "../TestCommon.h"
#include "ConnectionHelper.h"


/**
 * SelectionTS  -"test suite" class
 * Intended to test selective SQL commands
 * (classes Ora::Selector, Ora:::Rowset);
 */
class SelectionTS : public ::testing::Test,
                            ConnectionHelper
{
    using CH = ConnectionHelper;

public:
    SelectionTS() {
    }

    ~SelectionTS() {
    }

protected:
    void selectAndFetchToStdOut(const std::string& srtStmt)
    {
        bool bSucceeded(true);

        try
        {
            scope_t<> scope([](bool) {
                std::cout << std::endl;
            } );

            Ora::Selector select(spConn_(), srtStmt);
            auto rowset = select.select();

            Ora::RowReaderToStream rvals(spConn_(), std::cout);
            while (rowset.fetchNext(rvals)) {
            }

            scope.enclose();
        }
        catch (const std::exception& e)
        {
            bSucceeded = false;
            std::cerr << "SelectionTS::selectAndFetchToStdOut(): " << e.what() << std::endl;
        }

        ASSERT_TRUE(bSucceeded);
    }


    void SetUp()  override  {
        CH::SetUp();
    }
    void TearDown()  override {
        CH::TearDown();
    }

    const Ora::ConnectionSp& spConn_() const {
        return CH::spConn_;
    };

protected:
    std::unique_ptr<Ora::RowReaderToStream> spRowReader_;
};




#endif //HRTOSQLITE_ROOT_SELECTIONTS_H
