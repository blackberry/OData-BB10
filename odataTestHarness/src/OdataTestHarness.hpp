// Tabbed pane project template
#ifndef OdataTestHarness_HPP_
#define OdataTestHarness_HPP_

#include <QObject>

namespace bb {
    namespace cascades {
        class Application;
    }
}
class ODataSource;

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class OdataTestHarness: public QObject {
Q_OBJECT
public:
    OdataTestHarness(bb::cascades::Application *app);
    virtual ~OdataTestHarness();
};

#endif /* OdataTestHarness_HPP_ */
