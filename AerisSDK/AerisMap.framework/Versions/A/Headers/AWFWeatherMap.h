//
//  AFWeatherMap.h
//  AerisMap
//
//  Created by Nicholas Shipes on 10/16/13.
//  Copyright (c) 2013 HAMweather, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AWFWeatherMapConfig.h"
#import "AWFWeatherMapDelegate.h"
#import "AWFLayerType.h"

typedef NS_ENUM (NSUInteger, AWFWeatherMapType) {
	AWFWeatherMapTypeApple = 0,
	AWFWeatherMapTypeGoogle,
	AWFWeatherMapTypeMapbox,
	AWFWeatherMapTypeDefault = AWFWeatherMapTypeApple
};

@class AWFDataLayer, AWFAnimationTimeline;

/**
 *  An `AWFWeatherMap` object provides a complete interface for displaying weather-related overlays and data on a map view. You
 *	use this class to display and control weather information and map contents being displayed from your application.
 *
 *	When you initialize a weather map, you should specify the mapping library to use by using the appropriate {@link AWFWeatherMapType}
 *	value. Alternatively, you can initialize a weather map with a map view instance and the proper {@link AWFWeatherMapType} will be
 *	assigned based on the type of map view object provided.
 */
@interface AWFWeatherMap : NSObject

/**
 *  The type of mapping library being used with the weather map.
 *
 *	This value indicates the mapping library currently being used with the weather map.
 */
@property (readonly, nonatomic, assign) AWFWeatherMapType weatherMapType;

/**
 *  The view containing the map view and all related subviews, such as legends and controls.
 */
@property (readonly, nonatomic, strong) UIView *weatherMapView;

/**
 *  The starting date for the map's animation timeline.
 */
@property (nonatomic, strong) NSDate *timelineStartDate;

/**
 *  The ending date for the animation's timeline.
 */
@property (nonatomic, strong) NSDate *timelineEndDate;

/**
 *  The current date of the animation's timeline for which data is being displayed no the map.
 */
@property (readonly, nonatomic) NSDate *timelineCurrentTime;

/**
 *  The map view managed by this weather map instance.
 */
@property (readonly, nonatomic, strong) id mapView;

/**
 *  An object that needs to receive all delegate messages from the specific mapping SDK being used.
 */
@property (nonatomic, weak) id mapViewDelegate;

/**
 *  The configuration object associated with the weather map.
 */
@property (readonly, nonatomic, strong) AWFWeatherMapConfig *config;

/**
 *  The receiver's delegate.
 *
 *	A weather map sends messages to its delegate regarding the addition and removal of weather overlays, animation status changes, and forwards
 *	respective messages from the interval map view object.
 */
@property (nonatomic, assign) id <AWFWeatherMapDelegate> delegate;
@property (nonatomic, assign) id <AWFWeatherMapDataSource> dataSource;

/**
 *  A Boolean value indicating whether the weather map is currently animating.
 */
@property (readonly, nonatomic, getter = isAnimating) BOOL animating;

/**
 *  A Boolean value indicating whether the weather map is currently loading data for the animation.
 */
@property (readonly, nonatomic, getter = isLoadingAnimation) BOOL loadingAnimation;

/**
 *  Initializes and returns a newly allocated weather map object with the specified map type.
 *
 *  @param mapType The {@link AWFWeatherMapType} to use with the weather map.
 *
 *  @return An initialized weather map object or `nil` if the object couldn't be created.
 */
- (instancetype)initWithMapType:(AWFWeatherMapType)mapType;

/**
 *  Initializes and returns a newly allocated weather map object with the specified map type and configuration.
 *
 *  @param mapType   The {@link AWFWeatherMapType} to use with the weather map.
 *  @param mapConfig The configuration object to use with the weather map.
 *
 *  @return An initialized weather map object or `nil` if the object couldn't be created.
 */
- (instancetype)initWithMapType:(AWFWeatherMapType)mapType config:(AWFWeatherMapConfig *)mapConfig;

/**
 *  Initializes and returns a newly allocated weather map object using the specified map view.
 *
 *  @param mapView The map view to associate with the weather map.
 *
 *  @return An initialized weather map object or `nil` if the object couldn't be created.
 */
- (instancetype)initWithMapView:(id)mapView;

/**
 *  Initializes and returns a newly allocated weather map object with a map configuration using the specified map view.
 *
 *  @param mapView   The map view to associate with the weather map.
 *  @param mapConfig The configuration object to use with the weather map.
 *
 *  @return An initialized weather map object or `nil` if the object couldn't be created.
 */
- (instancetype)initWithMapView:(id)mapView config:(AWFWeatherMapConfig *)mapConfig;

/**
 *  Returns an array of layer types currently active on the map.
 *
 *  @return The array of active layer types.
 */
- (NSArray *)activeLayerTypes;

//-----------------------------------------------------------------------------
// @name Managing Overlay Types
//-----------------------------------------------------------------------------

/**
 *  Adds a single layer type to the map.
 *
 *  @param layerType The {@link AFLayerType} to add to the map.
 */
- (void)addLayerType:(AWFLayerType)layerType;

/**
 *  Adds an array of layer types to the map.
 *
 *  @param layerTypes The array of layer types to add to the map.
 */
- (void)addLayerTypes:(NSArray *)layerTypes;

/**
 *  Adds an layer type to the map above an existing layer type.
 *
 *  @param layerType      The {@link AFLayerType} to add to the map.
 *  @param otherLayerType The {@link AFLayerType} to insert the target layer type above.
 */
- (void)addLayerType:(AWFLayerType)layerType aboveLayerType:(AWFLayerType)otherLayerType;

/**
 *  Adds an layer type to the map above an existing layer type.
 *
 *  @param layerType The {@link AFLayerType} to add to the map.
 *  @param overlay     The overlay to insert the target layer type above. This object must conform to the MKOverlay protocol.
 */
- (void)addLayerType:(AWFLayerType)layerType aboveOverlay:(AWFDataLayer *)overlay;

/**
 *  Adds an layer type to the map below an existing layer type.
 *
 *  @param layerType      The {@link AFLayerType} to add to the map.
 *  @param otherLayerType The {@link AFLayerType} to insert the target layer type below.
 */
- (void)addLayerType:(AWFLayerType)layerType belowLayerType:(AWFLayerType)otherLayerType;

/**
 *  Adds an layer type to the map below an existing layer type.
 *
 *  @param layerType The {@link AFLayerType} to add to the map.
 *  @param overlay     The overlay to insert the target layer type below. This object must conform to the MKOverlay protocol.
 */
- (void)addLayerType:(AWFLayerType)layerType belowOverlay:(AWFDataLayer *)overlay;

/**
 *  Inserts an layer type to the map at the specified index.
 *
 *  @param layerType The {@link AFLayerType} to add to the map.
 *  @param index       The index at which to place the overlay. If this value is greater than the number of overlays on the map, the overlay will be
 * inserted at the top of the overlay stack.
 */
- (void)insertLayerType:(AWFLayerType)layerType atIndex:(NSUInteger)index;

/**
 *  Removes an layer type from the map.
 *
 *  @param layerType The {@link AFLayerType} to remove from the map.
 */
- (void)removeLayerType:(AWFLayerType)layerType;

/**
 *  An array of layer types to remove from the map.
 *
 *  @param layerTypes The array of layer types to remove.
 */
- (void)removeLayerTypes:(NSArray *)layerTypes;

/**
 *  Returns `YES` if the layer type exists on the map.
 *
 *  @param layerType The {@link AFLayerType} to check if it exists.
 *
 *  @return `YES` if the map contains the layer type, otherwise `NO`.
 */
- (BOOL)containsLayerType:(AWFLayerType)layerType;

/**
 *  Returns the overlay object associated with the specified layer type.
 *
 *  @param layerType The {@link AFLayerType} to return the overlay object for.
 *
 *  @return The `AWFDataLayer` instance for the specified layer type.
 */
- (AWFDataLayer *)dataLayerForType:(AWFLayerType)layerType;

//-----------------------------------------------------------------------------
// @name Updating Map Data
//-----------------------------------------------------------------------------

/**
 *  Updates the existing layer type.
 *
 *  @param layerType The {@link AFLayerType} to update on the map.
 */
- (void)refreshLayerType:(AWFLayerType)layerType;

/**
 *  Updates all existing layer types on the map.
 */
- (void)refreshAllLayerTypes;

/**
 *  Starts the auto-refresh timer, which will update all active overlays on the map at the specified interval in the weather map's
 *  configuration.
 *
 *  @see AWFWeatherMapConfig
 */
- (void)enableAutoRefresh;

/**
 *  Stops the auto-refresh timer that updates all active overlays at regular intervals.
 *
 *  It is recommended that your application call this method when the view containing the weather map disappears so that it no longer
 *  performs unnecessary processes.
 */
- (void)disableAutoRefresh;

- (void)updatePointDataForCurrentMapBounds;

//-----------------------------------------------------------------------------
// @name Animating Map Data
//-----------------------------------------------------------------------------

/**
 *  Starts animating active overlays from the beginning of the timeline.
 */
- (void)startAnimating;

/**
 *  Starts animating active overlays from the specified date.
 *
 *  @param time The date from which to begin animating.
 */
- (void)startAnimatingFromTime:(NSDate *)time;

/**
 *  Stops animating active overlays.
 */
- (void)stopAnimating;

/**
 *  Pauses all overlay animations if animating.
 */
- (void)pauseAnimation;

/**
 *  Moves the map timeline to the specified date and updates overlays as needed.
 *
 *  @param time The date and time to move the timeline to.
 */
- (void)goToTime:(NSDate *)time;

//-----------------------------------------------------------------------------
// @name Changing the Visible Region
//-----------------------------------------------------------------------------

/**
 *  Changes the center coordinate and zoom level of the map view and optionally animates the change.
 *
 *  @param centerCoordinate The new center coordinate for the map.
 *  @param zoomLevel        The new zoom level for the map.
 *  @param animated         Specify `YES` if you want the map view to scroll to the new location or `NO` if you want the map to display the new
 * location immediately.
 */
- (void)setMapCenterCoordinate:(CLLocationCoordinate2D)centerCoordinate zoomLevel:(NSUInteger)zoomLevel animated:(BOOL)animated;

//-----------------------------------------------------------------------------
// @name Presenting and Dismissing the Callout
//-----------------------------------------------------------------------------

- (void)showCalloutFromAnnotation:(id)annotation withTitle:(NSString *)title subtitle:(NSString *)subtitle;
- (void)showCalloutFromAnnotation:(id)annotation withContentView:(UIView *)contentView;
- (void)showCalloutAtCoordinate:(CLLocationCoordinate2D)coordinate withTitle:(NSString *)title subtitle:(NSString *)subtitle;
- (void)showCalloutAtCoordinate:(CLLocationCoordinate2D)coordinate withContentView:(UIView *)contentView;

//-----------------------------------------------------------------------------
// @name Class Methods
//-----------------------------------------------------------------------------

/**
 *  Returns a dictionary of all of the supported layer types.
 *
 *	The returned dictionary contains three objects, one for each data layer category (e.g. "tile", "point" and "polygon"). Each of these category
 * objects contains an
 *	array of dictionarys, one for each layer type within the category, and includes the overlay's code, type and name:
 *
 *	`{@"type": @(AWFLayerTypeRadar), @"code": @"radar", @"name": @"Radar"}`
 *
 *	This dictionary of supported layer types is often used to dynamically generate menus or listings of the available overlays {@link AWFWeatherMap}
 * supports.
 *
 *  @return The dictionary of supported layer types.
 */
+ (NSDictionary *)supportedDataLayers;

@end