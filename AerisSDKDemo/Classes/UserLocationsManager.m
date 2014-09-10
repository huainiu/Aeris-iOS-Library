//
//  UserLocationsManager.m
//  AerisCatalog
//
//  Created by Nicholas Shipes on 9/30/13.
//  Copyright (c) 2013 HAMweather, LLC. All rights reserved.
//

#import "UserLocationsManager.h"

@interface UserLocationsManager ()
- (NSDictionary *)placeToDictionary:(AWFPlace *)place;
- (void)updateUserPreferences;
@end

static NSString *prefsLocationsKey			= @"locations";
static NSString *prefsDefaultLocationKey	= @"defaultLocation";

static NSString *nameKey	= @"name";
static NSString *stateKey	= @"state";
static NSString *countryKey	= @"country";
static NSString *latKey		= @"lat";
static NSString *lonKey		= @"lon";

@implementation UserLocationsManager

+ (UserLocationsManager *)sharedManager {
	static UserLocationsManager *_sharedManager = nil;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		_sharedManager = [[UserLocationsManager alloc] init];
	});
	
	return _sharedManager;
}

#pragma mark - Instance Methods

- (id)init {
	self = [super init];
	if (self) {
		
		NSMutableArray *places = [[NSMutableArray alloc] init];
		NSArray *locations = [[NSUserDefaults standardUserDefaults] objectForKey:prefsLocationsKey];
		
		// convert all stored dictionary representations back to AWFPlace instances
		[locations enumerateObjectsUsingBlock:^(NSDictionary *loc, NSUInteger idx, BOOL *stop) {
			NSString *name = ([loc objectForKey:nameKey]) ? [loc objectForKey:nameKey] : nil;
			NSString *state = ([loc objectForKey:stateKey]) ? [loc objectForKey:stateKey] : nil;
			NSString *country = ([loc objectForKey:countryKey]) ? [loc objectForKey:countryKey] : nil;
			
			AWFPlace *place = [AWFPlace placeWithCity:name state:state country:country];
			place.latitude = ([loc objectForKey:latKey]) ? [loc objectForKey:latKey] : nil;
			place.longitude = ([loc objectForKey:lonKey]) ? [loc objectForKey:lonKey] : nil;
			
			[places addObject:place];
		}];
		
		self.locations = places;
	}
	return self;
}

- (AWFPlace *)defaultLocation {
	if ([[NSUserDefaults standardUserDefaults] objectForKey:prefsDefaultLocationKey]) {
		NSDictionary *loc = [[NSUserDefaults standardUserDefaults] objectForKey:prefsDefaultLocationKey];
		
		NSString *name = ([loc objectForKey:nameKey]) ? [loc objectForKey:nameKey] : nil;
		NSString *state = ([loc objectForKey:stateKey]) ? [loc objectForKey:stateKey] : nil;
		NSString *country = ([loc objectForKey:countryKey]) ? [loc objectForKey:countryKey] : nil;
		
		AWFPlace *place = [AWFPlace placeWithCity:name state:state country:country];
		place.latitude = ([loc objectForKey:latKey]) ? [loc objectForKey:latKey] : nil;
		place.longitude = ([loc objectForKey:lonKey]) ? [loc objectForKey:lonKey] : nil;
		
		// make sure location exists in saved locations already
		if (![self containsLocation:place]) {
			[self saveLocation:place];
		}
		
		return place;
	}
	return nil;
}

- (void)setDefaultLocation:(AWFPlace *)place {
	[[NSUserDefaults standardUserDefaults] setObject:[self placeToDictionary:place] forKey:prefsDefaultLocationKey];
	
	// get the unique id for this user
	NSString *userId = [[Preferences sharedInstance] userIdentifier];
	NSString *token = [[NSUserDefaults standardUserDefaults] objectForKey:@"DeviceToken"];
	if (token && userId) {
		// send stored location with this device token id back to server for push notifications
		AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
		manager.responseSerializer = [AFJSONResponseSerializer serializer];
		NSDictionary *parameters = @{@"token": token, @"uuid": userId, @"place": [NSString stringWithFormat:@"%@,%@,%@", place.name, place.state, place.country]};
		[manager POST:@"http://apollo.urban10.net/aeris/ios/push/devicestore.php" parameters:parameters success:^(AFHTTPRequestOperation *operation, id responseObject) {
			//NSLog(@"JSON: %@", responseObject);
		} failure:^(AFHTTPRequestOperation *operation, NSError *error) {
			NSLog(@"Error: %@", error);
		}];
	}
}

- (void)removeDefaultLocation {
	[[NSUserDefaults standardUserDefaults] removeObjectForKey:prefsDefaultLocationKey];
}

- (void)saveLocation:(AWFPlace *)place {
	if (![self containsLocation:place]) {
		NSMutableArray *mutablePlaces = [self.locations mutableCopy];
		[mutablePlaces addObject:place];
		self.locations = [NSArray arrayWithArray:mutablePlaces];
		
		[self updateUserPreferences];
	}
}

- (void)removeLocation:(AWFPlace *)place {
	__block BOOL updatePrefs = NO;
	AWFPlace *defaultLocation = [self defaultLocation];
	NSMutableArray *mutablePlaces = [self.locations mutableCopy];
	[mutablePlaces enumerateObjectsUsingBlock:^(AWFPlace *p, NSUInteger idx, BOOL *stop) {
		if ([p.name isEqualToString:place.name] && [p.state isEqualToString:place.state] && [p.country isEqualToString:place.country]) {
			[mutablePlaces removeObject:p];
			
			if ([p isEqualToPlaceByComparingName:defaultLocation]) {
				[self removeDefaultLocation];
			}
			
			updatePrefs = YES;
			*stop = YES;
		}
	}];
	
	if (updatePrefs) {
		self.locations = [NSArray arrayWithArray:mutablePlaces];
		[self updateUserPreferences];
	}
}

- (BOOL)containsLocation:(AWFPlace *)place {
	__block BOOL contains = NO;
	[self.locations enumerateObjectsUsingBlock:^(AWFPlace *p, NSUInteger idx, BOOL *stop) {
		if ([p.name isEqualToString:place.name] && [p.state isEqualToString:place.state] && [p.country isEqualToString:place.country]) {
			contains = YES;
			*stop = YES;
		}
	}];
	
	return contains;
}

#pragma mark - Private

- (NSDictionary *)placeToDictionary:(AWFPlace *)place {
	NSString *state = (place.state) ? place.state : @"";
	NSString *country = (place.country) ? place.country : @"";
	NSNumber *lat = (place.latitude) ? place.latitude : @0;
	NSNumber *lon = (place.longitude) ? place.longitude : @0;
	
	return @{nameKey: place.name, stateKey: state, countryKey: country, latKey: lat, lonKey: lon};
}

- (void)updateUserPreferences {
	NSMutableArray *locations = [[NSMutableArray alloc] init];
	[self.locations enumerateObjectsUsingBlock:^(AWFPlace *place, NSUInteger idx, BOOL *stop) {
		[locations addObject:[self placeToDictionary:place]];
	}];
	
	[[NSUserDefaults standardUserDefaults] setObject:locations forKey:prefsLocationsKey];
	[[NSUserDefaults standardUserDefaults] synchronize];
}

@end
