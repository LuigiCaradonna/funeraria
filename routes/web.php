<?php

use Illuminate\Foundation\Application;
use Illuminate\Support\Facades\Route;
use Inertia\Inertia;

use App\Http\Controllers\TombTypeController;
use App\Http\Controllers\TombController;
use App\Http\Controllers\ClientController;
use App\Http\Controllers\AccessoryTypeController;
use App\Http\Controllers\AccessoryController;
use App\Http\Controllers\SculptureController;
use App\Http\Controllers\MaterialController;
use App\Http\Controllers\FontController;
use App\Http\Controllers\ColorController;
use App\Http\Controllers\FinishingController;
use App\Http\Controllers\DecorationController;
use App\Http\Controllers\OutlineController;
use App\Http\Controllers\TombFrameController;
use App\Http\Controllers\PictureController;

Route::get('/', function () {
    return Inertia::render('Home', [
        'canLogin' => Route::has('login'),
        'canRegister' => Route::has('register')
    ]);
})->name('home');

Route::middleware(['auth', 'verified'])->group(function () {

    /****************** TombTypes' routes ******************/
    Route::get('/tomb_types', [TombTypeController::class, 'index'])->name('tomb_types');
    Route::get('/tomb_types/create', [TombTypeController::class, 'create'])->name('create_tomb_type');
    Route::post('/tomb_types/create', [TombTypeController::class, 'store'])->name('store_tomb_type');
    Route::patch('/tomb_types/{id}', [TombTypeController::class, 'edit'])->name('edit_tomb_type');
    Route::delete('/tomb_types/multidel', [TombTypeController::class, 'multiDelete'])->name('delete_tomb_types');
    Route::delete('/tomb_types/{id}', [TombTypeController::class, 'delete'])->name('delete_tomb_type');

    /****************** Tombs' routes ******************/
    Route::get('/tombs', [TombController::class, 'index'])->name('tombs');
    Route::get('/tombs/create', [TombController::class, 'create'])->name('create_tomb');
    Route::post('/tombs/create', [TombController::class, 'store'])->name('store_tomb');
    Route::patch('/tombs/{id}', [TombController::class, 'edit'])->name('edit_tomb');
    Route::delete('/tombs/multidel', [TombController::class, 'multiDelete'])->name('delete_tombs');
    Route::delete('/tombs/{id}', [TombController::class, 'delete'])->name('delete_tomb');

    /****************** Clients' routes ******************/
    Route::get('/clients', [ClientController::class, 'index'])->name('clients');
    Route::get('/clients/create', [ClientController::class, 'create'])->name('create_client');
    Route::post('/clients/create', [ClientController::class, 'store'])->name('store_client');
    Route::patch('/clients/{id}', [ClientController::class, 'edit'])->name('edit_client');
    Route::delete('/clients/multidel', [ClientController::class, 'multiDelete'])->name('delete_clients');
    Route::delete('/clients/{id}', [ClientController::class, 'delete'])->name('delete_client');

    /****************** AccessoryTypes' routes ******************/
    Route::get('/accessory_types', [AccessoryTypeController::class, 'index'])->name('accessory_types');
    Route::get('/accessory_types/create', [AccessoryTypeController::class, 'create'])->name('create_accessory_type');
    Route::post('/accessory_types/create', [AccessoryTypeController::class, 'store'])->name('store_accessory_type');
    Route::patch('/accessory_types/{id}', [AccessoryTypeController::class, 'edit'])->name('edit_accessory_type');
    Route::delete('/accessory_types/multidel', [AccessoryTypeController::class, 'multiDelete'])->name('delete_accessory_types');
    Route::delete('/accessory_types/{id}', [AccessoryTypeController::class, 'delete'])->name('delete_accessory_type');

    /****************** Accessories' routes ******************/
    Route::get('/accessories', [AccessoryController::class, 'index'])->name('accessories');
    Route::get('/accessories/create', [AccessoryController::class, 'create'])->name('create_accessory');
    Route::post('/accessories/create', [AccessoryController::class, 'store'])->name('store_accessory');
    Route::patch('/accessories/{id}', [AccessoryController::class, 'edit'])->name('edit_accessory');
    Route::delete('/accessories/multidel', [AccessoryController::class, 'multiDelete'])->name('delete_accessories');
    Route::delete('/accessories/{id}', [AccessoryController::class, 'delete'])->name('delete_accessory');

    /****************** Sculptures' routes ******************/
    Route::get('/sculptures', [SculptureController::class, 'index'])->name('sculptures');
    Route::get('/sculptures/create', [SculptureController::class, 'create'])->name('create_sculpture');
    Route::post('/sculptures/create', [SculptureController::class, 'store'])->name('store_sculpture');
    Route::patch('/sculptures/{id}', [SculptureController::class, 'edit'])->name('edit_sculpture');
    Route::delete('/sculptures/multidel', [SculptureController::class, 'multiDelete'])->name('delete_sculptures');
    Route::delete('/sculptures/{id}', [SculptureController::class, 'delete'])->name('delete_sculpture');

    /****************** Materials' routes ******************/
    Route::get('/materials', [MaterialController::class, 'index'])->name('materials');
    Route::get('/materials/create', [MaterialController::class, 'create'])->name('create_material');
    Route::post('/materials/create', [MaterialController::class, 'store'])->name('store_material');
    Route::patch('/materials/{id}', [MaterialController::class, 'edit'])->name('edit_material');
    Route::delete('/materials/multidel', [MaterialController::class, 'multiDelete'])->name('delete_materials');
    Route::delete('/materials/{id}', [MaterialController::class, 'delete'])->name('delete_material');

    /****************** Fonts' routes ******************/
    Route::get('/fonts', [FontController::class, 'index'])->name('fonts');
    Route::get('/fonts/create', [FontController::class, 'create'])->name('create_font');
    Route::post('/fonts/create', [FontController::class, 'store'])->name('store_font');
    Route::patch('/fonts/{id}', [FontController::class, 'edit'])->name('edit_font');
    Route::delete('/fonts/multidel', [FontController::class, 'multiDelete'])->name('delete_fonts');
    Route::delete('/fonts/{id}', [FontController::class, 'delete'])->name('delete_font');

    /****************** Colors' routes ******************/
    Route::get('/colors', [ColorController::class, 'index'])->name('colors');
    Route::get('/colors/create', [ColorController::class, 'create'])->name('create_color');
    Route::post('/colors/create', [ColorController::class, 'store'])->name('store_color');
    Route::patch('/colors/{id}', [ColorController::class, 'edit'])->name('edit_color');
    Route::delete('/colors/multidel', [ColorController::class, 'multiDelete'])->name('delete_colors');
    Route::delete('/colors/{id}', [ColorController::class, 'delete'])->name('delete_color');

    /****************** Finishings' routes ******************/
    Route::get('/finishings', [FinishingController::class, 'index'])->name('finishings');
    Route::get('/finishings/create', [FinishingController::class, 'create'])->name('create_finishing');
    Route::post('/finishings/create', [FinishingController::class, 'store'])->name('store_finishing');
    Route::patch('/finishings/{id}', [FinishingController::class, 'edit'])->name('edit_finishing');
    Route::delete('/finishings/multidel', [FinishingController::class, 'multiDelete'])->name('delete_finishings');
    Route::delete('/finishings/{id}', [FinishingController::class, 'delete'])->name('delete_finishing');
    
    /****************** Decorations' routes ******************/
    Route::get('/decorations', [DecorationController::class, 'index'])->name('decorations');
    Route::get('/decorations/create', [DecorationController::class, 'create'])->name('create_decorations');
    Route::post('/decorations/create', [DecorationController::class, 'store'])->name('store_decorations');
    Route::patch('/decorations/{id}', [DecorationController::class, 'edit'])->name('edit_decorations');
    Route::delete('/decorations/multidel', [DecorationController::class, 'multiDelete'])->name('delete_decorations');
    Route::delete('/decorations/{id}', [DecorationController::class, 'delete'])->name('delete_decoration');
    
    /****************** Outlines' routes ******************/
    Route::get('/outlines', [OutlineController::class, 'index'])->name('outlines');
    Route::get('/outlines/create', [OutlineController::class, 'create'])->name('create_outlines');
    Route::post('/outlines/create', [OutlineController::class, 'store'])->name('store_outlines');
    Route::patch('/outlines/{id}', [OutlineController::class, 'edit'])->name('edit_outlines');
    Route::delete('/outlines/multidel', [OutlineController::class, 'multiDelete'])->name('delete_outlines');
    Route::delete('/outlines/{id}', [OutlineController::class, 'delete'])->name('delete_outline');

    /****************** TombFrames' routes ******************/
    Route::get('/tomb_frames', [TombFrameController::class, 'index'])->name('tomb_frames');
    Route::get('/tomb_frames/create', [TombFrameController::class, 'create'])->name('create_tomb_frames');
    Route::post('/tomb_frames/create', [TombFrameController::class, 'store'])->name('store_tomb_frames');
    Route::patch('/tomb_frames/{id}', [TombFrameController::class, 'edit'])->name('edit_tomb_frames');
    Route::delete('/tomb_frames/multidel', [TombFrameController::class, 'multiDelete'])->name('delete_tomb_frames');
    Route::delete('/tomb_frames/{id}', [TombFrameController::class, 'delete'])->name('delete_tomb_frame');
    
    /****************** Pictures' routes ******************/
    Route::get('/pictures', [PictureController::class, 'index'])->name('pictures');
    Route::get('/pictures/create', [PictureController::class, 'create'])->name('create_pictures');
    Route::post('/pictures/create', [PictureController::class, 'store'])->name('store_pictures');
    Route::patch('/pictures/{id}', [PictureController::class, 'edit'])->name('edit_pictures');
    Route::delete('/pictures/multidel', [PictureController::class, 'multiDelete'])->name('delete_pictures');
    Route::delete('/pictures/{id}', [PictureController::class, 'delete'])->name('delete_picture');
});

// Auth routes
require __DIR__.'/auth.php';
