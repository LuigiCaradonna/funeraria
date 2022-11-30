<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('tombs', function (Blueprint $table) {
            $table->id();
            $table->unsignedTinyInteger('client_id')->constrained();
            $table->string('name');
            $table->unsignedDecimal('price',10,2)->nullable();
            $table->float('full_width')->nullable();
            $table->float('full_height')->nullable();
            $table->unsignedTinyInteger('tomb_type_id')->constrained(); // 2+2 cassettone/base-tetto, 3 ...
            $table->unsignedTinyInteger('finishing_id')->constrained(); // toro, toro-dritto, costa dritta
            $table->unsignedTinyInteger('material_id')->constrained();
            $table->unsignedTinyInteger('font_id')->constrained();
            $table->unsignedTinyInteger('color_id')->constrained();
            $table->unsignedSmallInteger('sculpture_id')->constrained();
            $table->boolean('polished')->nullable(); // polished sculpture or not
            $table->unsignedSmallInteger('vase_id')->constrained('accessory');         // vase
            $table->unsignedSmallInteger('lamp_id')->constrained('accessory');         // lamp
            $table->unsignedSmallInteger('flame_id')->constrained('accessory');        // flame
            $table->unsignedSmallInteger('photo_id')->constrained('accessory');        // photo
            $table->unsignedSmallInteger('frame_id')->constrained('accessory');        // frame
            $table->unsignedSmallInteger('cross_id')->constrained('accessory');        // cross
            $table->unsignedSmallInteger('decoration_id')->constrained('accessory');   // other decorations
            $table->text('note');
            $table->date('ordered_at');
            $table->date('proofed_at')->nullable();
            $table->date('confirmed_at')->nullable();
            $table->date('done_at')->nullable();
            $table->date('delivered_at')->nullable();
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('tombs');
    }
};
