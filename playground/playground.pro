TEMPLATE = subdirs

SUBDIRS += \
    anchors \
    dials \
    dialogbuttons \
    invoker \
    inputpanel \
    images

SUBDIRS += shadows

qtHaveModule(webengine) {

    SUBDIRS += \
        webview
}

qtHaveModule(quickwidgets) {

    SUBDIRS += \
        grids
}
