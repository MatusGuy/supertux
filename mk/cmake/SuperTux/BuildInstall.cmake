if(WIN32 AND NOT UNIX)
  if(VCPKG_BUILD)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/" DESTINATION ${INSTALL_SUBDIR_BIN} FILES_MATCHING PATTERN "*.dll")
  else()
    install(FILES ${DLLS} DESTINATION ${INSTALL_SUBDIR_BIN})
  endif()

  install(FILES ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.png ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.ico DESTINATION ".")

  option(PACKAGE_VCREDIST "Package the VCREDIST libraries with the program" OFF)

  if(PACKAGE_VCREDIST)
    set(CMAKE_INSTALL_UCRT_LIBRARIES true)
    include(InstallRequiredSystemLibraries)
  endif()

else()
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND DISABLE_CPACK_BUNDLING)

    set(INFOPLIST_CFBUNDLEEXECUTABLE "supertux2")

    configure_file(${PROJECT_SOURCE_DIR}/tools/darwin/info.plist.in ${CMAKE_BINARY_DIR}/tools/darwin/info.plist)
    install(FILES ${CMAKE_BINARY_DIR}/tools/darwin/info.plist DESTINATION "SuperTux.app/Contents/")
    install(FILES ${PROJECT_SOURCE_DIR}/tools/darwin/receipt DESTINATION "SuperTux.app/Contents/_MASReceipt/")

    install(FILES ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.png ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.icns DESTINATION "SuperTux.app/Contents/Resources/")

  else()

    if(UBUNTU_TOUCH)
      set(LINUX_DESKTOP_ICON "assets/supertux-256x256.png")
      # FIXME: The "install" folder is a folder managed by Clickable and shouldn't be hardcoded here
      configure_file(${PROJECT_SOURCE_DIR}/supertux2.desktop.in "install/supertux2.desktop")
      install(FILES ${PROJECT_SOURCE_DIR}/mk/clickable/supertux2.apparmor DESTINATION ".")
      configure_file(${PROJECT_SOURCE_DIR}/mk/clickable/manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/install/manifest.json)
      set(APPS "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/supertux2")
      install(FILES ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux-256x256.png DESTINATION "assets")
    else()
      set(LINUX_DESKTOP_ICON "supertux2")
      configure_file(${PROJECT_SOURCE_DIR}/supertux2.desktop.in "supertux2.desktop")
      install(FILES ${CMAKE_BINARY_DIR}/supertux2.desktop DESTINATION "share/applications")
      set(APPS "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_SUBDIR_BIN}/supertux2")
      install(FILES ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.png ${PROJECT_SOURCE_DIR}/data/images/engine/icons/supertux.xpm DESTINATION "share/pixmaps/")
    endif()

    install(FILES ${PROJECT_SOURCE_DIR}/supertux2.svg DESTINATION "share/icons/hicolor/scalable/apps")

  endif()
endif()

if(UBUNTU_TOUCH)
  install(TARGETS supertux2 DESTINATION ".")
else()
  install(TARGETS supertux2 DESTINATION ${INSTALL_SUBDIR_BIN})
endif()

if(EMSCRIPTEN)
  configure_file(${PROJECT_SOURCE_DIR}/mk/emscripten/template.html.in ${CMAKE_CURRENT_BINARY_DIR}/template.html)
  configure_file(${PROJECT_SOURCE_DIR}/mk/emscripten/supertux2.png ${CMAKE_CURRENT_BINARY_DIR}/supertux2.png COPYONLY)
  configure_file(${PROJECT_SOURCE_DIR}/mk/emscripten/supertux2_bkg.png ${CMAKE_CURRENT_BINARY_DIR}/supertux2_bkg.png COPYONLY)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/INSTALL.md ${PROJECT_SOURCE_DIR}/README.md ${PROJECT_SOURCE_DIR}/LICENSE.txt ${PROJECT_SOURCE_DIR}/NEWS.md DESTINATION ${INSTALL_SUBDIR_DOC})

install(FILES ${PROJECT_SOURCE_DIR}/data/credits.stxt DESTINATION ${INSTALL_SUBDIR_SHARE})

install(FILES ${PROJECT_SOURCE_DIR}/supertux2.appdata.xml DESTINATION "share/metainfo" )

install(DIRECTORY
  ${PROJECT_SOURCE_DIR}/data/images
  ${PROJECT_SOURCE_DIR}/data/fonts
  ${PROJECT_SOURCE_DIR}/data/music
  ${PROJECT_SOURCE_DIR}/data/particles
  ${PROJECT_SOURCE_DIR}/data/scripts
  ${PROJECT_SOURCE_DIR}/data/shader
  ${PROJECT_SOURCE_DIR}/data/speech
  ${PROJECT_SOURCE_DIR}/data/sounds
  ${PROJECT_SOURCE_DIR}/data/locale
  DESTINATION ${INSTALL_SUBDIR_SHARE})

if(CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/levels
    DESTINATION ${INSTALL_SUBDIR_SHARE}
    PATTERN "data/levels/test" EXCLUDE
    PATTERN "data/levels/test_old" EXCLUDE
    PATTERN "data/levels/incubator" EXCLUDE
    PATTERN "data/levels/misc/menu.stl.in" EXCLUDE)
else()
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/levels
    DESTINATION ${INSTALL_SUBDIR_SHARE}
    PATTERN "data/levels/misc/menu.stl.in" EXCLUDE)
endif()

install(FILES "${CMAKE_BINARY_DIR}/data/levels/misc/menu.stl" DESTINATION "${INSTALL_SUBDIR_SHARE}/levels/misc")

# move some config clutter to the advanced section
mark_as_advanced(
  INSTALL_SUBDIR_BIN
  INSTALL_SUBDIR_SHARE
  INSTALL_SUBDIR_DOC
  )

# EOF #
