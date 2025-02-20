project(generator)

include_directories(
  ${OMIM_ROOT}/3party/gflags/src
  ${OMIM_ROOT}/3party/jansson/src
)

set(
  SRC
  altitude_generator.cpp
  altitude_generator.hpp
  booking_dataset.cpp
  booking_dataset.hpp
  booking_scoring.cpp
  boost_helpers.hpp
  borders.cpp
  borders.hpp
  brands_loader.cpp
  brands_loader.hpp
  camera_info_collector.cpp
  camera_info_collector.hpp
  centers_table_builder.cpp
  centers_table_builder.hpp
  check_model.cpp
  check_model.hpp
  cities_boundaries_builder.cpp
  cities_boundaries_builder.hpp
  cities_boundaries_checker.cpp
  cities_boundaries_checker.hpp
  cities_ids_builder.cpp
  cities_ids_builder.hpp
  city_roads_generator.cpp
  city_roads_generator.hpp
  coastlines_generator.cpp
  coastlines_generator.hpp
  collection_base.hpp
  collector_addresses.cpp
  collector_addresses.hpp
  collector_camera.cpp
  collector_camera.hpp
  collector_collection.cpp
  collector_collection.hpp
  collector_interface.hpp
  collector_tag.cpp
  collector_tag.hpp
  data_version.cpp
  data_version.hpp
  descriptions_section_builder.cpp
  descriptions_section_builder.hpp
  dumper.cpp
  dumper.hpp
  emitter_booking.hpp
  emitter_coastline.cpp
  emitter_coastline.hpp
  emitter_country.cpp
  emitter_country.hpp
  emitter_factory.hpp
  emitter_interface.hpp
  emitter_noop.hpp
  emitter_restaurants.cpp
  emitter_restaurants.hpp
  emitter_simple.cpp
  emitter_simple.hpp
  emitter_world.cpp
  emitter_world.hpp
  factory_utils.hpp
  feature_builder.cpp
  feature_builder.hpp
  feature_emitter_iface.hpp
  feature_generator.cpp
  feature_generator.hpp
  feature_helpers.cpp
  feature_helpers.hpp
  feature_maker.cpp
  feature_maker.hpp
  feature_maker_base.cpp
  feature_maker_base.hpp
  feature_merger.cpp
  feature_merger.hpp
  feature_processing_layers.cpp
  feature_processing_layers.hpp
  feature_sorter.cpp
  feature_sorter.hpp
  filter_collection.cpp
  filter_collection.hpp
  filter_elements.cpp
  filter_elements.hpp
  filter_interface.hpp
  filter_planet.cpp
  filter_planet.hpp
  gen_mwm_info.hpp
  generate_info.hpp
  geo_objects/geo_objects_generator.cpp
  geo_objects/geo_objects_generator.hpp
  geo_objects/geo_objects_maintainer.cpp
  geo_objects/geo_objects_maintainer.hpp
  geo_objects/geo_objects.cpp
  geo_objects/geo_objects.hpp
  geo_objects/geo_objects_filter.cpp
  geo_objects/geo_objects_filter.hpp
  geometry_holder.hpp
  holes.cpp
  holes.hpp
  intermediate_data.cpp
  intermediate_data.hpp
  intermediate_elements.hpp
  key_value_storage.cpp
  key_value_storage.hpp
  locality_sorter.cpp
  locality_sorter.hpp
  maxspeeds_builder.cpp
  maxspeeds_builder.hpp
  maxspeeds_collector.cpp
  maxspeeds_collector.hpp
  maxspeeds_parser.cpp
  maxspeeds_parser.hpp
  metalines_builder.cpp
  metalines_builder.hpp
  node_mixer.cpp
  node_mixer.hpp
  opentable_dataset.cpp
  opentable_dataset.hpp
  opentable_scoring.cpp
  osm2meta.cpp
  osm2meta.hpp
  osm2type.cpp
  osm2type.hpp
  osm_element.cpp
  osm_element.hpp
  osm_element_helpers.cpp
  osm_element_helpers.hpp
  osm_o5m_source.hpp
  osm_source.cpp
  osm_xml_source.hpp
  place.cpp
  place.hpp
  place_processor.cpp
  place_processor.hpp
  place_node.hpp
  platform_helpers.cpp
  platform_helpers.hpp
  polygonizer.hpp
  popular_places_section_builder.cpp
  popular_places_section_builder.hpp
  popularity.cpp
  popularity.hpp
  promo_catalog_cities.hpp
  ratings_section_builder.cpp
  ratings_section_builder.hpp
  region_meta.cpp
  region_meta.hpp
  regions/admin_suburbs_marker.cpp
  regions/admin_suburbs_marker.hpp
  regions/collector_region_info.cpp
  regions/collector_region_info.hpp
  regions/country_specifier.cpp
  regions/country_specifier.hpp
  regions/country_specifier_builder.cpp
  regions/country_specifier_builder.hpp
  regions/level_region.hpp
  regions/locality_point_integrator.cpp
  regions/locality_point_integrator.hpp
  regions/node.cpp
  regions/node.hpp
  regions/place_point.hpp
  regions/place_points_integrator.cpp
  regions/place_points_integrator.hpp
  regions/region.cpp
  regions/region.hpp
  regions/region_base.cpp
  regions/region_base.hpp
  regions/region_info.cpp
  regions/region_info.hpp
  regions/region_info_getter.cpp
  regions/region_info_getter.hpp
  regions/regions.cpp
  regions/regions.hpp
  regions/regions_builder.cpp
  regions/regions_builder.hpp
  relation_tags.cpp
  relation_tags.hpp
  relation_tags_enricher.cpp
  relation_tags_enricher.hpp
  restriction_collector.cpp
  restriction_collector.hpp
  restriction_generator.cpp
  restriction_generator.hpp
  restriction_writer.cpp
  restriction_writer.hpp
  road_access_generator.cpp
  road_access_generator.hpp
  routing_helpers.cpp
  routing_helpers.hpp
  routing_index_generator.cpp
  routing_index_generator.hpp
  search_index_builder.cpp
  search_index_builder.hpp
  sponsored_dataset.hpp
  sponsored_dataset_inl.hpp
  sponsored_object_base.hpp
  sponsored_object_storage.hpp
  sponsored_scoring.cpp
  sponsored_scoring.hpp
  srtm_parser.cpp
  srtm_parser.hpp
  statistics.cpp
  statistics.hpp
  streets/street_geometry.cpp
  streets/street_geometry.hpp
  streets/street_regions_tracing.cpp
  streets/street_regions_tracing.hpp
  streets/streets.cpp
  streets/streets.hpp
  streets/streets_builder.cpp
  streets/streets_builder.hpp
  streets/streets_filter.cpp
  streets/streets_filter.hpp
  tag_admixer.hpp
  tesselator.cpp
  tesselator.hpp
  towns_dumper.cpp
  towns_dumper.hpp
  translation.hpp
  translation.cpp
  traffic_generator.cpp
  traffic_generator.hpp
  transit_generator.cpp
  transit_generator.hpp
  translator.cpp
  translator.hpp
  translator_coastline.cpp
  translator_coastline.hpp
  translator_collection.cpp
  translator_collection.hpp
  translator_country.cpp
  translator_country.hpp
  translator_factory.hpp
  translator_geo_objects.cpp
  translator_geo_objects.hpp
  translator_interface.hpp
  translator_region.cpp
  translator_region.hpp
  translator_streets.cpp
  translator_streets.hpp
  translator_world.cpp
  translator_world.hpp
  type_helper.cpp
  type_helper.hpp
  ugc_db.cpp
  ugc_db.hpp
  ugc_section_builder.cpp
  ugc_section_builder.hpp
  ugc_translator.cpp
  ugc_translator.hpp
  unpack_mwm.cpp
  unpack_mwm.hpp
  utils.cpp
  utils.hpp
  ways_merger.cpp
  ways_merger.hpp
  wiki_url_dumper.cpp
  wiki_url_dumper.hpp
  world_map_generator.hpp
)

omim_add_library(${PROJECT_NAME} ${SRC})

omim_add_test_subdirectory(generator_tests_support)
omim_add_test_subdirectory(generator_tests)

add_subdirectory(generator_tool)
add_subdirectory(booking_quality_check)
add_subdirectory(extract_addr)
add_subdirectory(feature_segments_checker)
add_subdirectory(srtm_coverage_checker)
