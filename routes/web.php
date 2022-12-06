<?php

use Illuminate\Foundation\Application;
use Illuminate\Support\Facades\Route;
use Inertia\Inertia;

use App\Http\Controllers\TombTypeController;
use App\Http\Controllers\TombController;
use App\Http\Controllers\ClientController;
use App\Http\Controllers\FlameController;
use App\Http\Controllers\LampController;
use App\Http\Controllers\VaseController;
use App\Http\Controllers\CrossController;
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

    /****************** Flames' routes ******************/
    Route::get('/flames', [FlameController::class, 'index'])->name('flames');
    Route::get('/flames/create', [FlameController::class, 'create'])->name('create_flame');
    Route::post('/flames/create', [FlameController::class, 'store'])->name('store_flame');
    Route::patch('/flames/{id}', [FlameController::class, 'edit'])->name('edit_flame');
    Route::delete('/flames/multidel', [FlameController::class, 'multiDelete'])->name('delete_flames');
    Route::delete('/flames/{id}', [FlameController::class, 'delete'])->name('delete_flame');

    /****************** Lamps' routes ******************/
    Route::get('/lamps', [LampController::class, 'index'])->name('lamps');
    Route::get('/lamps/create', [LampController::class, 'create'])->name('create_lamp');
    Route::post('/lamps/create', [LampController::class, 'store'])->name('store_lamp');
    Route::patch('/lamps/{id}', [LampController::class, 'edit'])->name('edit_lamp');
    Route::delete('/lamps/multidel', [LampController::class, 'multiDelete'])->name('delete_lamps');
    Route::delete('/lamps/{id}', [LampController::class, 'delete'])->name('delete_lamp');

    /****************** Vases' routes ******************/
    Route::get('/vases', [VaseController::class, 'index'])->name('vases');
    Route::get('/vases/create', [VaseController::class, 'create'])->name('create_vase');
    Route::post('/vases/create', [VaseController::class, 'store'])->name('store_vase');
    Route::patch('/vases/{id}', [VaseController::class, 'edit'])->name('edit_vase');
    Route::delete('/vases/multidel', [VaseController::class, 'multiDelete'])->name('delete_vases');
    Route::delete('/vases/{id}', [VaseController::class, 'delete'])->name('delete_vase');

    /****************** Crosses' routes ******************/
    Route::get('/crosses', [CrossController::class, 'index'])->name('crosses');
    Route::get('/crosses/create', [CrossController::class, 'create'])->name('create_cross');
    Route::post('/crosses/create', [CrossController::class, 'store'])->name('store_cross');
    Route::patch('/crosses/{id}', [CrossController::class, 'edit'])->name('edit_cross');
    Route::delete('/crosses/multidel', [CrossController::class, 'multiDelete'])->name('delete_crosses');
    Route::delete('/crosses/{id}', [CrossController::class, 'delete'])->name('delete_cross');

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
