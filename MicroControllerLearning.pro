TEMPLATE = subdirs

SUBDIRS = controller \
        test_app \
        utils \
        dispatch_service

#libs
controller.subdir = libs/controller
utils.subdir = libs/utils

#test app
test_app.subdir = test_app
