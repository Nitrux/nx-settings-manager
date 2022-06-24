#include "modulesmanager.h"

#include <QDir>
#include <QDebug>
#include <QQmlEngine>
#include <QVector>
#include "code/abstractmodule.h"

//modules
#include "modules/background/backgroundmodule.h"
#include "modules/theme/thememodule.h"
#include "modules/about/aboutmodule.h"

#include <MauiMan/mauimanutils.h>

ModulesManager::ModulesManager(QObject *parent) : QObject(parent)
  ,m_model(new ModulesModel(this))
{
    //    QString path = MAUIMAN_MODULES_IMPORT_PATH;
    //    QQmlEngine engine;
    auto server = new MauiManUtils(this);
    m_serverRunning = server->serverRunning();

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        m_serverRunning = state;
        emit serverRunningChanged(m_serverRunning);
    });

    m_model->appendModule(new AboutModule);

    m_model->appendModule(new BackgroundModule);
    m_model->appendModule(new ThemeModule);
}

ModulesModel *ModulesManager::model() const
{
    return m_model;
}

bool ModulesManager::serverRunning() const
{
    return m_serverRunning;
}

void ModulesManager::startServer()
{
    MauiManUtils::startServer();
}

AbstractModule* ModulesManager::sourceFor(const QString &moduleName)
{
    for(const auto &module : m_model->modules())
    {
        if(module->name() == moduleName)
        {
            return module;
        }
    }

    return nullptr;
}
